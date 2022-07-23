//
// AED, November 2021
//
// Solution of the first practical assignement (subset sum problem)
//
// Daniel Ferreira – 102442
// Pedro Rasinhas – 103541
// Guilherme Antunes – 103600
//

#if __STDC_VERSION__ < 199901L
#error "This code must must be compiled in c99 mode or later (-std=c99)" // to handle the unsigned long long data type
#endif
#ifndef STUDENT_H_FILE
#define STUDENT_H_FILE "103600.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include "elapsed_time.h"
#include STUDENT_H_FILE

typedef struct
{
    integer_t som;
    integer_t comb;
    int ind1;
    int ind2;
} sum;

void progConfig()
{
    double time = cpu_time();
    fprintf(stderr, "Program configuration:\n");
    fprintf(stderr, "  min_n ....... %d\n", min_n);
    fprintf(stderr, "  max_n ....... %d\n", max_n);
    fprintf(stderr, "  n_sums ...... %d\n", n_sums);
    fprintf(stderr, "  n_problems .. %d\n", n_problems);
    fprintf(stderr, "  integer_t ... %d bits\n", 8 * (int)sizeof(integer_t));

    for (int i = 0; i < n_problems; i++)
    {
        int n = all_subset_sum_problems[i].n;

        if (n > 25)
            continue;
        integer_t *ptr = all_subset_sum_problems[i].p;

        printf("\nPara o problema %d: \n", n);
        printf("Os valores de p são: ");

        for (int c = 0; c < n; c++)
        {

            printf("%lld, ", ptr[c]);
        }
        printf("\n\n");
        printf("Os valores das somas são: ");

        for (int s = 0; s < n_sums; s++)
        {

            printf("%lld, ", all_subset_sum_problems[i].sums[s]);
        }
        printf("\n\n");
    }
    printf("progConfig %f\n", cpu_time() - time);
}

void insertion_sort(sum *data, int first, int one_after_last)
{
    int i, j;
    for (i = first + 1; i < one_after_last; i++)
    {
        sum temp = data[i];
        for (j = i; j > first && temp.som < data[j - 1].som; j--)
            data[j] = data[j - 1];
        data[j] = temp;
    }
}

void Merge(sum *data, int first, int one_after_last)
{
    int i, j, k, middle;
    sum *buffer;
    if (one_after_last - first < 40)
        insertion_sort(data, first, one_after_last);
    else
    {
        middle = (first + one_after_last) / 2;
        Merge(data, first, middle);
        Merge(data, middle, one_after_last);
        buffer = (sum *)malloc((size_t)(one_after_last - first) * sizeof(sum)) - first;
        i = first;                                                                      
        j = middle;                                                                     
        k = first;                                                                      
        while (k < one_after_last)
            if (j == one_after_last || (i < middle && data[i].som <= data[j].som))
                buffer[k++] = data[i++];
            else
                buffer[k++] = data[j++];
        for (i = first; i < one_after_last; i++)
            data[i] = buffer[i];
        free(buffer + first);
    }
}

void insertMax(sum heap[], sum soma, int size)
{
    int i;
    for (i = size; i > 0 && heap[(i - 1) / 2].som < soma.som; i = (i - 1) / 2)
        heap[i] = heap[(i - 1) / 2];
    heap[i] = soma;
}

void insertMin(sum heap[], sum soma, int size)
{
    int i;
    for (i = size; i > 0 && heap[(i - 1) / 2].som > soma.som; i = (i - 1) / 2)
        heap[i] = heap[(i - 1) / 2];
    heap[i] = soma;
}

sum removeMax(sum heap[], int size)
{
    sum r = heap[0];
    int i2=0;
    int i = 0;
    while (i * 2 + 1 <= size)
    {
        i2 = i * 2 + 1;
        if (i2 < size && heap[i2].som < heap[i2 + 1].som)
            i2++;
        if (heap[i2].som > heap[size].som)
            heap[i] = heap[i2];
        else
            break;
        i = i2;
    }
    heap[i] = heap[size];
    return r;
}

sum removeMin(sum heap[], int size)
{
    sum r = heap[0];
    int i2 = 0;
    int i = 0;
    while (i * 2 + 1 <= size)
    {
        i2 = i * 2 + 1;
        if (i2 < size && heap[i2].som > heap[i2 + 1].som)
            i2++;
        if (heap[i2].som < heap[size].som)
            heap[i] = heap[i2];
        else
            break;
        i = i2;
    }
    heap[i] = heap[size];
    return r;
}

int somas(int n, integer_t *vals, sum *sums)
{
    int i;
    for (integer_t a = 0; a < 1 << n; a++)
    {
        for (i = 0; i < n; i++)
        {
            if (a & (1 << i))
            {
                sums[a].som += vals[i];
                sums[a].comb = sums[a].comb | (1 << i);
            }
        }
    }
    return 1;
}

int schroeppel(integer_t desired, sum minHeap[], sum maxHeap[], int nA, int nB, int nC, int nD, sum sumsA[], sum sumsB[], sum sumsC[], sum sumsD[], int nP, int nS)
{
    while (nP > 0 && nS > 0)
    {
        if (maxHeap[0].som + minHeap[0].som == desired)
        {
            integer_t b = minHeap[0].comb | (maxHeap[0].comb << (nB + nA));
            for (int i = 0; i < nA + nB + nC + nD; i++)
            {
                printf("%s", b & 1 ? "1" : "0");
                b = b >> 1;
            }
            printf("\n");
            return 1;
        }
        else if (maxHeap[0].som + minHeap[0].som > desired)
        {
            sum r = removeMax(maxHeap, --nS);
            r.ind2--;
            if (r.ind2 >= 0)
            {
                sum s = {
                    .som = sumsC[r.ind1].som + sumsD[r.ind2].som,
                    .comb = sumsC[r.ind1].comb | (sumsD[r.ind2].comb << nC),
                    .ind1 = r.ind1,
                    .ind2 = r.ind2};
                insertMax(maxHeap, s, nS++);
            }
        }
        else
        {
            sum r = removeMin(minHeap, --nP);
            r.ind1++;
            if (r.ind1 < (1 << nA))
            {
                sum s = {
                    .som = sumsA[r.ind1].som + sumsB[r.ind2].som,
                    .comb = sumsA[r.ind1].comb | (sumsB[r.ind2].comb << nA),
                    .ind1 = r.ind1,
                    .ind2 = r.ind2};
                insertMin(minHeap, s, nP++);
            }
        }
    }

    return 0;
}

int main(void)
{
    progConfig();

    for (int i = 0; i < n_problems; i++)
    {
        if (i <= 1)
        {
            int n = all_subset_sum_problems[i].n;
            integer_t *p = all_subset_sum_problems[i].p;
            double somaTempos = 0;
            double tempoMaximo = 0;

            int nB = n / 2;
            int nD = n - nB;
            int nA = nB / 2;
            nB -= nA;
            int nC = nD / 2;
            nD -= nC;

            sum *sumsA = malloc((1 << nA) * sizeof(sum));
            sum *sumsB = malloc((1 << nB) * sizeof(sum));
            sum *sumsC = malloc((1 << nC) * sizeof(sum));
            sum *sumsD = malloc((1 << nD) * sizeof(sum));

            somas(nA, p, sumsA);
            somas(nB, p + nA, sumsB);
            somas(nC, p + nA + nB, sumsC);
            somas(nD, p + nA + nB + nC, sumsD);

            Merge(sumsA, 0, (1 << nA) - 1);
            Merge(sumsB, 0, (1 << nB) - 1);
            Merge(sumsC, 0, (1 << nC) - 1);
            Merge(sumsD, 0, (1 << nD) - 1);

            for (int j = 0; j < n_sums; j++)
            {
                integer_t desired = all_subset_sum_problems[i].sums[j];
                double time = cpu_time();
                sum minHeap[1 << nB];
                int nP = 0;
                for (int a = 0; a < 1 << nB; a++)
                {
                    sum s = {
                        .som = sumsA[0].som + sumsB[a].som,
                        .comb = sumsA[0].comb | (sumsB[a].comb << nA),
                        .ind1 = 0,
                        .ind2 = a};
                    insertMin(minHeap, s, nP++);
                }
                sum maxHeap[1 << nC];
                int nS = 0;
                for (int a = 0; a < 1 << nC; a++)
                {
                    sum s = {
                        .som = sumsC[a].som + sumsD[(1 << nD) - 1].som,
                        .comb = sumsC[a].comb | (sumsD[(1 << nD) - 1].comb << nC),
                        .ind1 = a,
                        .ind2 = (1 << nD) - 1};
                    insertMax(maxHeap, s, nS++);
                }
                if (!schroeppel(desired, minHeap, maxHeap, nA, nB, nC, nD, sumsA, sumsB, sumsC, sumsD, nP, nS))
                {
                    printf("\nSoluçao NÃO encontrada!\n");
                    break;
                }

                time = cpu_time() - time;
                somaTempos += time;
                if (time > tempoMaximo)
                    tempoMaximo = time;
            }
            free(sumsA);
            free(sumsB);
            free(sumsC);
            free(sumsD);

            double tempoMedio = somaTempos / n_sums;
            printf("%d %f %f\n\n", i + 10, tempoMedio, tempoMaximo);
        }
    }
    return 0;
}