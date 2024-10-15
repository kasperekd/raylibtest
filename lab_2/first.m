% Главная программа
clear;
C = 3 * 10 ^ 8; % Скорость света, м/с
THERMAL_NOISE_FLOOR_DBM_HZ = -174; % Уровень теплового шума, дБм/Гц

power_bs_transmitter_dbm = 46;
sectors_per_bs = 3;
power_ue_transmitter_dbm = 24;
antenna_gain_bs_dbi = 21;
penetration_loss_db = 15;
interference_margin_db = 1;
frequency_ghz = 1.8;
bandwidth_ul_mhz = 10;
bandwidth_dl_mhz = 20;
noise_figure_bs_db = 2.4;
noise_figure_ue_db = 6;
sinr_dl_db = 2;
sinr_ul_db = 4;
mimo_antennas_bs = 2;
area_total_km2 = 100;
area_business_centers_km2 = 4;
feeder_loss_db = 2;

h_BS = 50; % Высота базовой станции
h_MS = 3; % Высота мобильной станции

r_umi = 500; % m
r_walf = 5000; % m

end_d = 29000; % Конечное расстояние для расчётов

% 1. Вычисляем параметры: thermal noise, sensitivity, MIMO gain
[thermal_noise_dl, thermal_noise_ul, sensitivity_bs_dbm, sensitivity_ue_dbm, mimo_gain_db] = ...
    computeParameters(THERMAL_NOISE_FLOOR_DBM_HZ, bandwidth_dl_mhz, bandwidth_ul_mhz, sinr_dl_db, sinr_ul_db, noise_figure_bs_db, noise_figure_ue_db, mimo_antennas_bs);

% 2. Расчёт максимальных потерь пути
[max_path_loss_dl, max_path_loss_ul] = ...
    calculateMaxPathLoss(power_bs_transmitter_dbm, feeder_loss_db, antenna_gain_bs_dbi, mimo_gain_db, ...
    penetration_loss_db, interference_margin_db, sensitivity_ue_dbm, power_ue_transmitter_dbm, sensitivity_bs_dbm);

fprintf('Уровень максимально допустимых потерь сигнала MAPL_UL: %.2f дБ\n', max_path_loss_ul);
fprintf('Уровень максимально допустимых потерь сигнала MAPL_DL: %.2f дБ\n', max_path_loss_dl);

% 3. Модели распространения сигнала
distances = (1:end_d);
path_loss_umi = calculateUMiNLOS(frequency_ghz, distances);
path_loss_fspl = calculateFSPL(frequency_ghz, distances, C);

path_loss_cost_DU = calculateCost231(distances, frequency_ghz, h_BS, h_MS, "DU");
path_loss_cost_U = calculateCost231(distances, frequency_ghz, h_BS, h_MS, "U");
path_loss_cost_SU = calculateCost231(distances, frequency_ghz, h_BS, h_MS, "SU");
path_loss_cost_RURAL = calculateCost231(distances, frequency_ghz, h_BS, h_MS, "RURAL");
path_loss_cost_ROAD = calculateCost231(distances, frequency_ghz, h_BS, h_MS, "ROAD");

% 4. Поиск точек пересечения моделей с MAPL_UL
intersection_umi_ul = findIntersection(distances, path_loss_umi, max_path_loss_ul);
intersection_fspl_ul = findIntersection(distances, path_loss_fspl, max_path_loss_ul);

intersection_cost_ul_DU = findIntersection(distances, path_loss_cost_DU, max_path_loss_ul);
intersection_cost_ul_U = findIntersection(distances, path_loss_cost_U, max_path_loss_ul);
intersection_cost_ul_SU = findIntersection(distances, path_loss_cost_SU, max_path_loss_ul);
intersection_cost_ul_RURAL = findIntersection(distances, path_loss_cost_RURAL, max_path_loss_ul);
intersection_cost_ul_ROAD = findIntersection(distances, path_loss_cost_ROAD, max_path_loss_ul);

% % 5. Построение графиков
% plotGraph1(distances, path_loss_umi, path_loss_fspl, path_loss_cost, max_path_loss_dl, max_path_loss_ul, ...
%     intersection_umi_ul, intersection_fspl_ul, intersection_cost_ul);

plotGraph1(distances, path_loss_umi, path_loss_fspl, max_path_loss_dl, max_path_loss_ul, ...
    intersection_umi_ul, intersection_fspl_ul, 'UMiNLOS и FSPL');

plotGraph2(distances, path_loss_cost_DU, path_loss_cost_U, path_loss_cost_SU, ...
    path_loss_cost_RURAL, path_loss_cost_ROAD, max_path_loss_dl, max_path_loss_ul, ...
    intersection_cost_ul_DU, intersection_cost_ul_U, intersection_cost_ul_SU, ...
    intersection_cost_ul_RURAL, intersection_cost_ul_ROAD);

% 6. Расчёт покрытия базовой станции
r_umi = intersection_umi_ul;
r_walf = intersection_cost_ul_SU;
calculateCoverage(area_business_centers_km2, area_total_km2, r_umi, r_walf);

%% Функции %%

% thermal noise, sensitivity, MIMO gain
function [thermal_noise_dl, thermal_noise_ul, sensitivity_bs_dbm, sensitivity_ue_dbm, mimo_gain_db] = ...
        computeParameters(THERMAL_NOISE_FLOOR_DBM_HZ, bandwidth_dl_mhz, bandwidth_ul_mhz, sinr_dl_db, sinr_ul_db, noise_figure_bs_db, noise_figure_ue_db, mimo_antennas_bs)

    bandwidth_dl_hz = bandwidth_dl_mhz * 10 ^ 6;
    bandwidth_ul_hz = bandwidth_ul_mhz * 10 ^ 6;

    thermal_noise_dl = THERMAL_NOISE_FLOOR_DBM_HZ + 10 * log10(bandwidth_dl_hz);
    thermal_noise_ul = THERMAL_NOISE_FLOOR_DBM_HZ + 10 * log10(bandwidth_ul_hz);

    sensitivity_bs_dbm = thermal_noise_dl + sinr_dl_db + noise_figure_bs_db;
    sensitivity_ue_dbm = thermal_noise_ul + sinr_ul_db + noise_figure_ue_db;

    mimo_gain_db = 10 * log10(mimo_antennas_bs);
end

% MaxPathLoss
function [max_path_loss_dl, max_path_loss_ul] = ...
        calculateMaxPathLoss(power_bs_transmitter_dbm, feeder_loss_db, antenna_gain_bs_dbi, mimo_gain_db, ...
        penetration_loss_db, interference_margin_db, sensitivity_ue_dbm, power_ue_transmitter_dbm, sensitivity_bs_dbm)

    max_path_loss_dl = power_bs_transmitter_dbm - feeder_loss_db + antenna_gain_bs_dbi + mimo_gain_db - ...
        penetration_loss_db - interference_margin_db - sensitivity_ue_dbm;

    max_path_loss_ul = power_ue_transmitter_dbm - feeder_loss_db + antenna_gain_bs_dbi + mimo_gain_db - ...
        penetration_loss_db - interference_margin_db - sensitivity_bs_dbm;
end

% UMiNLOS
function path_loss_umi = calculateUMiNLOS(frequency_ghz, distances)
    path_loss_umi = 26 * log10(frequency_ghz) + 22.7 + 36.7 * log10(distances);
end

% FSPL
function path_loss_fspl = calculateFSPL(frequency_ghz, distances, C)
    frequency_hz = frequency_ghz * 10 ^ 9;
    path_loss_fspl = 20 * log10((4 * pi * frequency_hz * distances) / C);
end

% Функция расчёта модели COST 231
function path_loss_cost = calculateCost231(distances, frequency_ghz, h_BS, h_MS, terrain_type)
    A = 46.3;
    B = 33.9;
    path_loss_cost = zeros(size(distances));

    for i = 1:length(distances)
        distance_km = distances(i) * 10 ^ -3;

        % Расчет a_hms в зависимости от типа местности
        if strcmp(terrain_type, 'DU') || strcmp(terrain_type, 'U')
            a_hms = 3.2 * (log10(11.75 * h_MS)) ^ 2 - 4.97;
        elseif strcmp(terrain_type, 'SU') || strcmp(terrain_type, 'RURAL') || strcmp(terrain_type, 'ROAD')
            a_hms = (1.1 * log10(frequency_ghz * 10 ^ 3)) * h_MS - (1.56 * log10(frequency_ghz * 10 ^ 3) - 0.8);
        else
            error('Unknown terrain type');
        end

        % Расчет s в зависимости от расстояния
        if distance_km >= 1
            s = 44.9 - 6.55 * log10(frequency_ghz * 10 ^ 3);
        else
            s = (47.88 + 13.9 * log10(frequency_ghz * 10 ^ 3) - 13.9 * log10(h_BS)) * (1 / log10(50));
        end

        % Расчет L_clutter в зависимости от типа местности
        if strcmp(terrain_type, 'DU')
            L_clutter = 3;
        elseif strcmp(terrain_type, 'U')
            L_clutter = 0;
        elseif strcmp(terrain_type, 'SU')
            L_clutter =- (2 * (log10(frequency_ghz * 10 ^ 3/28)) ^ 2 + 5.4);
        elseif strcmp(terrain_type, 'RURAL')
            L_clutter =- (4.78 * (log10(frequency_ghz * 10 ^ 3)) ^ 2 - 18.33 * log10(frequency_ghz * 10 ^ 3) + 40.94);
        elseif strcmp(terrain_type, 'ROAD')
            L_clutter =- (4.78 * (log10(frequency_ghz * 10 ^ 3)) ^ 2 - 18.33 * log10(frequency_ghz * 10 ^ 3) + 35.94);
        else
            error('Unknown terrain type');
        end

        % Расчет потерь пути
        path_loss_cost(i) = A + B * log10(frequency_ghz * 10 ^ 3) - 13.82 * log10(h_BS) - a_hms + s * log10(distance_km) + L_clutter;
    end

end

% intersection
function intersection = findIntersection(distances, path_loss, mapl_ul)
    difference = abs(path_loss - mapl_ul);
    [~, idx] = min(difference); % Ищем минимальное расстояние между кривой и MAPL_UL
    intersection = distances(idx);
end

% для UMiNLOS и FSPL
function plotGraph1(distances, path_loss_umi, path_loss_fspl, max_path_loss_dl, max_path_loss_ul, ...
        intersection_umi_ul, intersection_fspl_ul, title_str)

    figure(1);
    h1 = plot(distances, path_loss_umi, 'DisplayName', 'UMiNLOS');
    hold on;
    h2 = plot(distances, path_loss_fspl, '--', 'DisplayName', 'FSPL');

    % Добавляем горизонтальные линии для MAPL
    h4 = yline(max_path_loss_dl, 'r', 'MAPL_DL', 'LineWidth', 1);
    h5 = yline(max_path_loss_ul, 'b', 'MAPL_UL', 'LineWidth', 1);

    % Добавляем вертикальные линии пересечения с MAPL_UL
    h6 = xline(intersection_umi_ul, 'g--', 'UMiNLOS Intersection', 'LineWidth', 1);
    h7 = xline(intersection_fspl_ul, 'm--', 'FSPL Intersection', 'LineWidth', 1);

    fprintf('UMiNLOS d,m: %.2f\n', intersection_umi_ul);
    fprintf('FSPL d,m: %.2f\n', intersection_fspl_ul);

    set(h4, 'HandleVisibility', 'off');
    set(h5, 'HandleVisibility', 'off');
    set(h6, 'HandleVisibility', 'off');
    set(h7, 'HandleVisibility', 'off');

    xlabel('Расстояние между приемником и передатчиком, м');
    ylabel('Потери сигнала, дБ');
    title(title_str);
    legend show;
    grid on;
    hold off;

end

function plotGraph2(distances, path_loss_cost_DU, path_loss_cost_U, path_loss_cost_SU, ...
        path_loss_cost_RURAL, path_loss_cost_ROAD, max_path_loss_dl, max_path_loss_ul, ...
        intersection_cost_ul_DU, intersection_cost_ul_U, intersection_cost_ul_SU, ...
        intersection_cost_ul_RURAL, intersection_cost_ul_ROAD)

    figure(2);

    % Построение графиков для различных типов местности
    h1 = plot(distances, path_loss_cost_DU, 'DisplayName', 'COST 231 DU');
    hold on;
    h2 = plot(distances, path_loss_cost_U, 'DisplayName', 'COST 231 U', 'LineStyle', '--');
    h3 = plot(distances, path_loss_cost_SU, 'DisplayName', 'COST 231 SU', 'LineStyle', ':');
    h4 = plot(distances, path_loss_cost_RURAL, 'DisplayName', 'COST 231 RURAL', 'LineStyle', '-.');
    h5 = plot(distances, path_loss_cost_ROAD, 'DisplayName', 'COST 231 ROAD', 'LineStyle', '-');

    % Добавляем горизонтальные линии для MAPL
    h6 = yline(max_path_loss_dl, 'r', 'MAPL_DL', 'LineWidth', 1);
    h7 = yline(max_path_loss_ul, 'b', 'MAPL_UL', 'LineWidth', 1);

    h8 = xline(intersection_cost_ul_DU, 'g--', 'COST 231 DU Intersection', 'LineWidth', 1);
    h9 = xline(intersection_cost_ul_U, 'm--', 'COST 231 U Intersection', 'LineWidth', 1);
    h10 = xline(intersection_cost_ul_SU, 'c--', 'COST 231 SU Intersection', 'LineWidth', 1);
    h11 = xline(intersection_cost_ul_RURAL, 'k--', 'COST 231 RURAL Intersection', 'LineWidth', 1);
    h12 = xline(intersection_cost_ul_ROAD, 'b--', 'COST 231 ROAD Intersection', 'LineWidth', 1);

    % Выводим точки пересечения в консоль
    fprintf('COST 231 DU d,m: %.2f\n', intersection_cost_ul_DU);
    fprintf('COST 231 U d,m: %.2f\n', intersection_cost_ul_U);
    fprintf('COST 231 SU d,m: %.2f\n', intersection_cost_ul_SU);
    fprintf('COST 231 RURAL d,m: %.2f\n', intersection_cost_ul_RURAL);
    fprintf('COST 231 ROAD d,m: %.2f\n', intersection_cost_ul_ROAD);

    set(h6, 'HandleVisibility', 'off');
    set(h7, 'HandleVisibility', 'off');
    set(h8, 'HandleVisibility', 'off');
    set(h9, 'HandleVisibility', 'off');
    set(h10, 'HandleVisibility', 'off');
    set(h11, 'HandleVisibility', 'off');
    set(h12, 'HandleVisibility', 'off');

    xlabel('Расстояние между приемником и передатчиком, м');
    ylabel('Потери сигнала, дБ');
    title('COST 231 для различных типов местности');
    legend show;
    grid on;
    hold off;

end

% Функция расчёта покрытия базовой станции
function calculateCoverage(area_business_centers_km2, area_total_km2, r_umi, r_walf)

    radius_umi = r_umi * 10 ^ -3; % Перевод в км
    radius_walf = r_walf * 10 ^ -3; % Перевод в км

    area_umi = 1.95 * radius_umi ^ 2;
    area_walf = 1.95 * radius_walf ^ 2;

    fprintf('Радиус Базовой станции для Микро = %.3f км\n', radius_umi);
    fprintf('Радиус Базовой станции для Макро = %.3f км\n', radius_walf);
    fprintf('Площадь одной базовой станции для Микро = %.3f км кв\n', area_umi);
    fprintf('Площадь одной базовой станции для Макро = %.3f км кв\n', area_walf);

    bs_count_umi = area_business_centers_km2 / area_umi;
    bs_count_walf = area_total_km2 / area_walf;

    fprintf('Необходимое количество базовых станций для Микро: %.2f\n', bs_count_umi);
    fprintf('Необходимое количество базовых станций для Макро: %.2f\n', bs_count_walf);
end
