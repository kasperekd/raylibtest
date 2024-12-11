#include "SignalAnalysis.h"

namespace SignalAnalysis {
double normalizedCorrelation(const BitSequence& seq1, const BitSequence& seq2) {
    if (seq1.size() != seq2.size()) {
        throw std::invalid_argument(
            "Sequence sizes must match for correlation.");
    }
    int N = seq1.size();
    double sumXY = 0.0, sumX2 = 0.0, sumY2 = 0.0;

    for (int n = 0; n < N; n++) {
        sumXY += seq1[n] * seq2[n];
        sumX2 += seq1[n] * seq1[n];
        sumY2 += seq2[n] * seq2[n];
    }

    return sumXY / (std::sqrt(sumX2) * std::sqrt(sumY2));
}

double Correlation(const BitSequence& seq1, const BitSequence& seq2) {
    if (seq1.size() != seq2.size()) {
        throw std::invalid_argument(
            "Sequence sizes must match for correlation.");
    }
    int N = seq1.size();
    double sum = 0.0;

    for (int n = 0; n < N; n++) {
        sum += seq1[n] * seq2[n];
    }

    return sum;

    // double correlation = 0.0;
    // for (size_t i = 0; i < seq1.size(); ++i) {
    //     correlation += (seq1[i] == seq2[i]) ? 1 : -1;
    // }
    // return correlation;
}
}  // namespace SignalAnalysis