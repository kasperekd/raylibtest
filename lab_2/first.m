clear;
C = 3 * 10^8;  % Скорость света, м/с
THERMAL_NOISE_FLOOR_DBM_HZ = -174;  % Уровень теплового шума, дБм/Гц

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

r_umi = 500; % m
r_walf = 5000; % m

end_d = 7000;

% Вычисляемые параметры
frequency_hz = frequency_ghz * 10^9;
bandwidth_ul_hz = bandwidth_ul_mhz * 10^6;
bandwidth_dl_hz = bandwidth_dl_mhz * 10^6;

% Расчет теплового шума
thermal_noise_dl = THERMAL_NOISE_FLOOR_DBM_HZ + 10 * log10(bandwidth_dl_hz);
thermal_noise_ul = THERMAL_NOISE_FLOOR_DBM_HZ + 10 * log10(bandwidth_ul_hz);

% Расчет чувствительности
sensitivity_bs_dbm = thermal_noise_dl + sinr_dl_db + noise_figure_bs_db;
sensitivity_ue_dbm = thermal_noise_ul + sinr_ul_db + noise_figure_ue_db;

% Вычисление MIMO усиления
mimo_gain_db = 10 * log10(mimo_antennas_bs);

% Расчет максимальных потерь пути
max_path_loss_dl = power_bs_transmitter_dbm - feeder_loss_db + antenna_gain_bs_dbi + mimo_gain_db - ...
    penetration_loss_db - interference_margin_db - sensitivity_ue_dbm;
max_path_loss_ul = power_ue_transmitter_dbm - feeder_loss_db + antenna_gain_bs_dbi + mimo_gain_db - ...
    penetration_loss_db - interference_margin_db - sensitivity_bs_dbm;

fprintf('Уровень максимально допустимых потерь сигнала MAPL_UL: %.2f дБ\n', max_path_loss_ul);
fprintf('Уровень максимально допустимых потерь сигнала MAPL_DL: %.2f дБ\n', max_path_loss_dl);

% Функции для расчета моделей распространения сигнала
distances = (1:end_d);
path_loss_umi = 26 * log10(frequency_ghz) + 22.7 + 36.7 * log10(distances);

% Расчет FSPL
path_loss_fspl = 20 * log10((4 * pi * frequency_hz * distances) / C);

% Расчет COST 231
path_loss_cost = zeros(size(distances));
for i = 1:length(distances)
    distance_km = distances(i) * 10^-3;
    path_loss_cost(i) = 46.3 + 33.9 * log10(frequency_ghz * 10^3) - 13.82 * log10(50) - ...
        (3.2 * (log10(11.75 * 1))^2 - 4.97) + ...
        (44.9 - 6.55 * log10(frequency_ghz * 10^3)) * log10(distance_km) + 3;
end

% Построение графика UMiNLOS
figure(1);
plot(distances, path_loss_umi, 'DisplayName', 'UMiNLOS');
hold on;
plot(distances, path_loss_fspl, '--', 'DisplayName', 'FSPM');
plot(distances, path_loss_cost, 'DisplayName', 'COST 231');
yline(max_path_loss_dl, 'r', 'MAPL_DL', 'LineWidth', 1);
yline(max_path_loss_ul, 'b', 'MAPL_UL', 'LineWidth', 1);
xlabel('Расстояние между приемником и передатчиком, м');
ylabel('Потери сигнала, дБ');
legend;
grid on;
hold off;

% Построение графика COST 231
distances_m = (1:end_d);
path_loss_cost = zeros(size(distances_m));
for i = 1:length(distances_m)
    distance_km = distances_m(i) * 10^-3;
    path_loss_cost(i) = 46.3 + 33.9 * log10(frequency_ghz * 10^3) - 13.82 * log10(50) - ...
        (3.2 * (log10(11.75 * 1))^2 - 4.97) + ...
        (44.9 - 6.55 * log10(frequency_ghz * 10^3)) * log10(distance_km) + 3;
end

figure(2);
plot(distances_m, path_loss_cost, 'DisplayName', 'COST 231');
hold on;
plot(distances_m, path_loss_fspl(1:end_d), '--', 'DisplayName', 'FSPM');
yline(max_path_loss_dl, 'r', 'MAPL_DL', 'LineWidth', 1);
yline(max_path_loss_ul, 'b', 'MAPL_UL', 'LineWidth', 1);
xlabel('Расстояние между приемником и передатчиком, м');
ylabel('Потери сигнала, дБ');
legend;
grid on;
hold off;

% Построение всех моделей
path_loss_walfish = 42.6 + 20 * log10(frequency_ghz * 10^3) + 26 * log10(distances / 1000);
path_loss_cost = zeros(size(distances));
for i = 1:length(distances)
    distance_km = distances(i) * 10^-3;
    path_loss_cost(i) = 46.3 + 33.9 * log10(frequency_ghz * 10^3) - 13.82 * log10(50) - ...
        (3.2 * (log10(11.75 * 1))^2 - 4.97) + ...
        (44.9 - 6.55 * log10(frequency_ghz * 10^3)) * log10(distance_km) + 3;
end

figure(3);
plot(distances, path_loss_cost, 'DisplayName', 'COST 231');
hold on;
plot(distances, path_loss_umi, 'DisplayName', 'UMiNLOS');
plot(distances, path_loss_walfish, 'DisplayName', 'Walfish-Ikegami');
yline(max_path_loss_dl, 'r', 'MAPL_DL', 'LineWidth', 1);
yline(max_path_loss_ul, 'b', 'MAPL_UL', 'LineWidth', 1);
xlabel('Расстояние между приемником и передатчиком, м');
ylabel('Потери сигнала, дБ');
legend;
grid on;
hold off;

% Расчет покрытия базовой станции
radius_umi = r_umi * 10^-3;
radius_walf = r_walf * 10^-3;

area_umi = 1.95 * radius_umi^2;
area_walf = 1.95 * radius_walf^2;

fprintf('Радиус Базовой станции для Макро = %.3f км\n', radius_umi);
fprintf('Радиус Базовой станции для Микро = %.3f км\n', radius_walf);
fprintf('Площадь одной базовой станции для Макро = %.3f км кв\n', area_umi);
fprintf('Площадь одной базовой станции для Микро = %.3f км кв\n', area_walf);

bs_count_umi = area_business_centers_km2 / area_umi;
bs_count_walf = area_total_km2 / area_walf;

fprintf('Необходимое количество базовых станций для Макро: %.2f\n', bs_count_umi);
fprintf('Необходимое количество базовых станций для Микро: %.2f\n', bs_count_walf);
