clear;

% Функция для сдвига регистра с обратной связью
function [new_reg, output_bit] = shift_register(state, feedback_indices)
    feedback_bit = 0;

    for index = feedback_indices
        feedback_bit = xor(feedback_bit, state(index));
    end

    output_bit = state(end); % Получаем выходной бит
    new_reg = [feedback_bit, state(1:end - 1)]; % Обновляем регистр
end

% Функция для создания последовательности Голда
function gold_seq = create_gold_sequence(init_x, init_y, feedback_x, feedback_y, seq_length)
    gold_seq = zeros(1, seq_length);

    for i = 1:seq_length
        [init_x, bit_x] = shift_register(init_x, feedback_x);
        [init_y, bit_y] = shift_register(init_y, feedback_y);
        gold_seq(i) = xor(bit_x, bit_y);
    end

end

% Начальные значения и параметры
initial_x = [0, 1, 1, 0, 1];
initial_y = [1, 0, 1, 0, 0];
feedback_positions_x = [1, 3];
feedback_positions_y = [1, 2, 3, 5];

% Альтернативные начальные значения
alternative_x = [0, 1, 1, 1, 0];
alternative_y = [0, 1, 1, 1, 1];

sequence_len = 31;

% Генерация последовательностей Голда
gold_seq_1 = create_gold_sequence(initial_x, initial_y, feedback_positions_x, feedback_positions_y, sequence_len);
gold_seq_2 = create_gold_sequence(alternative_x, alternative_y, feedback_positions_x, feedback_positions_y, sequence_len);

% Вывод первой последовательности
fprintf('Первая последовательность Голда: ');
fprintf('%d ', gold_seq_1);
fprintf('\n');

% Вычисление нормализованной автокорреляции
[autocorr_vals, delay] = xcorr(gold_seq_1, 'coeff');

% Фильтрация для удаления отрицательных сдвигов
non_negative_indices = delay >= 0;
filtered_delay = delay(non_negative_indices);
filtered_autocorr_vals = autocorr_vals(non_negative_indices);

% Формирование таблицы сдвигов и значений автокорреляции
correlation_table = [filtered_delay', filtered_autocorr_vals'];
disp('Сдвиг | Нормализованная автокорреляция');
disp(correlation_table);

% Вывод второй последовательности
fprintf('Вторая последовательность Голда: ');
fprintf('%d ', gold_seq_2);
fprintf('\n');

% Вычисление взаимной корреляции
[cross_corr_vals, cross_delay] = xcorr(gold_seq_1, gold_seq_2, 'coeff');
disp(['Максимальная взаимная корреляция: ', num2str(max(cross_corr_vals))]);

% Построение графика автокорреляции
figure(1);
plot(delay, autocorr_vals);
title('Нормализованная автокорреляция в зависимости от задержки');
xlabel('Задержка (lag)');
ylabel('Нормализованная автокорреляция');
grid on;
