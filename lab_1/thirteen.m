function quantization_analysis(signal, Fs, bit_depths)
    % Исходный сигнал
    N = length(signal);
    
    % Прямое дискретное преобразование Фурье для оригинального сигнала
    Y_original = fft(signal);
    
    % Определение оси частот
    f_axis = (0:N-1)*(Fs/N);
    
    % оригинального сигнала
    figure;
    plot(f_axis, abs(Y_original)/N);
    title('Амплитудный спектр оригинального сигнала');
    xlabel('Частота (Гц)');
    ylabel('Амплитуда');
    xlim([0 Fs/2]);
    grid on;

    % Анализ для каждой разрядности
    for i = 1:length(bit_depths)
        bit_depth = bit_depths(i);
        
        % Определение максимального значения для данной разрядности
        max_value = 2^bit_depth - 1;

        % Квантование сигнала
        quantized_signal = round(signal * max_value) / max_value;

        % Прямое дискретное преобразование Фурье для квантованного сигнала
        Y_quantized = fft(quantized_signal);

        % Визуализация амплитудного спектра квантованного сигнала
        figure;
        plot(f_axis, abs(Y_quantized)/N);
        title(['Амплитудный спектр квантованного сигнала (разрядность: ', num2str(bit_depth), ')']);
        xlabel('Частота (Гц)');
        ylabel('Амплитуда');
        xlim([0 Fs/2]);
        grid on;

        % Вычисление средней ошибки квантования
        quantization_error = signal - quantized_signal; % Ошибка квантования
        mean_quantization_error = mean(abs(quantization_error)); % Средняя ошибка
        disp(['Средняя ошибка квантования для разрядности ', num2str(bit_depth), ': ', num2str(mean_quantization_error)]);
    end
end

% Генерация сигнала
f = 9;
t = 0:0.001:1;
y = 6 * sin(2 * pi * f * t + pi/5);
Fs = 8000; % Частота дискретизации

% АЦП для анализа
bit_depths = [3, 4, 5, 6];

% Выполняем анализ
quantization_analysis(y, Fs, bit_depths);
