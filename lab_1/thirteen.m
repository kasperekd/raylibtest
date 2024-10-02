% Параметры сигнала
frequency = 9; % Частота
sampling_rate = 1000; % Частота дискретизации
time_vector = 0:1/sampling_rate:1; % Временной вектор
signal = 6 * sin(2 * pi * frequency * time_vector + pi/5); % Генерация сигнала

% Разрядности для анализа
bit_depths = [3, 4, 5, 6];

% Функция для квантования сигнала
function quantized_signal = quantize(y, num_bits)
num_levels = 2^num_bits - 1; % Количество уровней
min_val = min(y);
max_val = max(y);
normalized_signal = (y - min_val) / (max_val - min_val); % Нормализация
quantized_signal = round(normalized_signal * num_levels); % Квантование
quantized_signal = quantized_signal / num_levels * (max_val - min_val) + min_val; % Обратное масштабирование
end

% Анализ для каждой разрядности
for i = 1:length(bit_depths)
    quantized = quantize(signal, bit_depths(i));
    spectrum = fft(quantized);
    
    figure;
    plot(abs(spectrum));
    title(['Спектр сигнала с разрядностью ', num2str(bit_depths(i)), ' бита']);
end
