% Параметры
f1 = 13;
f2 = 17;
f3 = 27;
t = 0:0.01:1; % Временная ось

% Определение сигналов
s1 = cos(2 * pi * f1 * t);
s2 = cos(2 * pi * f2 * t);
s3 = cos(2 * pi * f3 * t);

% Определение сигналов a(t) и b(t)
a_signal = 5 * s1 + 4 * s2 + s3;
b_signal = s1 + (1/3) * s2;

% Вычисление корреляции между s1 и a, s1 и b
correlation_s1_a = corr(s1', a_signal');
correlation_s1_b = corr(s1', b_signal');

% Нормализованная корреляция
norm_correlation_s1_a = max(xcorr(s1, a_signal)) / (norm(s1) * norm(a_signal));
norm_correlation_s1_b = max(xcorr(s1, b_signal)) / (norm(s1) * norm(b_signal));

% Вывод результатов корреляции
disp(['Корреляция между s1 и a: ', num2str(correlation_s1_a)]);
disp(['Корреляция между s1 и b: ', num2str(correlation_s1_b)]);
disp(['Нормализованная корреляция между s1 и a: ', num2str(norm_correlation_s1_a)]);
disp(['Нормализованная корреляция между s1 и b: ', num2str(norm_correlation_s1_b)]);

% Заданные последовательности
a = [0.3, 0.2, -0.1, 4.2, -2, 1.5, 0];
b = [0.3, 4, -2.2, 1.6, 0.1, 0.1, 0.2];

% Взаимная корреляция со сдвигом
N = length(a);
max_corr = -Inf;
optimal_shift = 0;
correlation_values = zeros(1, N);

for shift = 0:N - 1
    b_shifted = circshift(b, shift);
    correlation_values(shift + 1) = sum(a .* b_shifted);

    if correlation_values(shift + 1) > max_corr
        max_corr = correlation_values(shift + 1);
        optimal_shift = shift;
    end

end

% Вывод оптимального сдвига
disp(['Оптимальный сдвиг для максимальной корреляции: ', num2str(optimal_shift)]);

% Построение графика корреляции
figure(1);
subplot(2, 1, 1);
stem(a);
title('Сигнал a');
xlabel('Номер отсчета');
ylabel('Значение');

subplot(2, 1, 2);
stem(b);
title('Сигнал b');
xlabel('Номер отсчета');
ylabel('Значение');

figure(2);
plot(correlation_values);
title('Зависимость корреляции от сдвига');
xlabel('Сдвиг');
ylabel('Значение корреляции');

% Построение графиков a и b с оптимальным сдвигом
figure(3);
subplot(2, 1, 1);
plot(a);
title('Сигнал a');
xlabel('Номер отсчета');
ylabel('Значение');

subplot(2, 1, 2);
b_shifted_optimal = circshift(b, optimal_shift);
plot(b_shifted_optimal);
title(['Сигнал b, сдвинутый на ', num2str(optimal_shift), ' шагов']);
xlabel('Номер отсчета');
ylabel('Значение');
