clear;
% Шаг 1: Генерация и визуализация непрерывного сигнала
% f = 9; % Частота
t = 0:0.0000001:1;
y = 6 * sin(2 * pi * 9 * t + pi/5);

figure;
plot(t, y);
title('Непрерывный сигнал y(t)');
xlabel('Время (с)');
ylabel('Амплитуда');
grid on;

% Шаг 2: Определение максимальной частоты в спектре сигнала
Fs = 1 / (t(2) - t(1));
N = length(y); % Длина сигнала
Y = fft(y);
P2 = abs(Y/N);
P1 = P2(1:floor(N/2)+1);
P1(2:end-1) = 2*P1(2:end-1); % Учет амплитуды

f = Fs*(0:floor(N/2))/N; % Вектор частот

% Находим максимальную частоту
[max_amplitude, max_index] = max(P1);
max_freq = round(f(max_index)); % Максимальная частота
% max_freq = f;

disp(['Максимальная частота в спектре: ', num2str(max_freq), ' Гц']);

% Шаг 3: Определение минимальной необходимой частоты дискретизации
fs_min = 2 * max_freq; % Частота по теореме Котельникова
disp(['Минимальная необходимая частота дискретизации: ', num2str(fs_min), ' Гц']);

% Шаг 4: Оцифровка сигнала
fs = fs_min;
t_sampled = 0:1/fs:1;
y_sampled = max_amplitude * sin(2 * pi * max_freq * t_sampled + pi/5); % Оцифрованный сигнал

clear Y N;
% Шаг 5: Прямое дискретное преобразование Фурье
Y = fft(y_sampled); % Дискретное преобразование Фурье
N = length(y_sampled); % Количество
f_axis = (0:N-1)*(fs/N);
Y_magnitude = abs(Y)/N; % Нормализация амплитуды

figure;
plot(f_axis, Y_magnitude);
title('Спектр сигнала');
xlabel('Частота (Гц)');
ylabel('Амплитуда');
xlim([0 fs/2]);
grid on;

% Ширина спектра
spectrum_width = max(f_axis) - min(f_axis);
disp(['Ширина спектра: ', num2str(spectrum_width), ' Гц'])

% Объем памяти для хранения массива
memory_size = N * 8; % Предполагаем, что используем double (8 байт)
disp(['Объем памяти, требуемый для хранения массива: ', num2str(memory_size), ' байт']);

% Шаг 6: Восстановление оригинального сигнала
t_reconstructed = linspace(0, 1, 1000); % Временной вектор для восстановления
y_reconstructed = interp1(t_sampled, y_sampled, t_reconstructed, 'linear'); % Интерполяция

% Визуализация оригинального и восстановленного сигнала
figure;
plot(t, y, 'b', 'DisplayName', 'Оригинальный сигнал');
hold on;
plot(t_reconstructed, y_reconstructed, 'r--', 'DisplayName', 'Восстановленный сигнал');
title('Сравнение оригинального и восстановленного сигнала');
xlabel('Время (с)');
ylabel('Амплитуда');
legend;
grid on;
