function quantization_effect()
% 1. Параметры сигнала
Fs = 8000; % Частота дискретизации
t = 0:1/Fs:1; % Временной вектор (1 секунда)
f_signal = 9; % Частота сигнала (6 Гц)
original_signal = 6 * sin(2 * pi * f_signal * t + pi/5); % Исходный сигнал

% 2. Разрядности АЦП
bit_depths = [3, 4, 5, 6];

% 3. Анализ и квантование
for bits = bit_depths
quantized_signal = quantize_signal(original_signal, bits);

% 4. Выполните ПДПФ для оригинального и квантованного сигналов
Y = fft(original_signal);
Y_quantized = fft(quantized_signal);

% 5. Вычислите амплитудные спектры
amplitude_spectrum = abs(Y);
amplitude_spectrum_quantized = abs(Y_quantized);

% Нормализация амплитудных спектров
amplitude_spectrum = amplitude_spectrum / max(amplitude_spectrum);
amplitude_spectrum_quantized = amplitude_spectrum_quantized / max(amplitude_spectrum_quantized);

% 6. Вычислите среднюю ошибку квантования
quantization_error = original_signal - quantized_signal;
mean_quantization_error = mean(abs(quantization_error));

% 7. Визуализация амплитудных спектров
f = (0:length(original_signal)-1) * (Fs / length(original_signal));

figure;
plot(f(1:length(amplitude_spectrum)/2), amplitude_spectrum(1:length(amplitude_spectrum)/2), 'b', 'DisplayName', 'Оригинальный сигнал');
hold on;
plot(f(1:length(amplitude_spectrum_quantized)/2), amplitude_spectrum_quantized(1:length(amplitude_spectrum_quantized)/2), 'r', 'DisplayName', sprintf('Квантованный сигнал (биты = %d)', bits));
hold off;
title(sprintf('Сравнение амплитудных спектров для %d бит', bits));
xlabel('Частота (Гц)');
ylabel('Нормализованная амплитуда');
xlim([0 Fs/2]);
legend show;
grid on;

% 8. Вывод средней ошибки квантования
fprintf('Средняя ошибка квантования для %d бит: %.4f\n', bits, mean_quantization_error);
end
end

function quantized_signal = quantize_signal(signal, bits)
% Определите максимальное и минимальное значения для квантования
max_value = 4; % Максимальная амплитуда
min_value = -4; % Минимальная амплитуда

% Определите количество уровней квантования
levels = 2^bits;

% Масштабируйте сигнал для диапазона уровней
scaled_signal = (signal - min_value) / (max_value - min_value) * (levels - 1);
scaled_signal = round(scaled_signal); % Округление до ближайшего уровня
scaled_signal(scaled_signal >= levels) = levels - 1; % Ограничьте уровень
quantized_signal = scaled_signal / (levels - 1) * (max_value - min_value) + min_value; % Обратное масштабирование
end