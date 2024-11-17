#include <stdio.h>
#include <stdlib.h>

 // ������ �ҷ����� �Լ�
int loadfile(double age[], double goals[], double assists[], double value[], int *count);
//����� ��� �Լ�
int invertmatrix(double matrix[4][4], double inverse[4][4]);
// ���Թ����� ��� �Լ�
int linear_regression(double age[], double goals[], double assists[], double value[], double slope[], int count);
// ���߼���ȸ�� ��� �Լ�
double predict(double slope[], double age, double goals, double assists);

//int input()
int main() {
    int count = 0;
    double age[100];
    double goals[100];
    double assists[100];
    double value[100];
    double slope[4];
    double EstValue = 0.0;

    loadfile(age, goals, assists, value, &count);

    linear_regression(age, goals, assists, value, slope, count);

    printf("�� ���α׷��� �౸ ���ݼ��� ����,��,��ý�Ʈ �� �Է��ϸ� ���� ���尡ġ�� �����ݴϴ�\n\n");

    printf("���� ȸ�� �� �Ķ���� ��� ");
    printf("\n������� �� �Ķ����%lf\n",slope[0]);
    printf("������ �� �Ķ���� %lf\n",slope[1]);
    printf("�� ���� �� �Ķ���� %lf\n",slope[2]);
    printf("��ý�Ʈ ���� �� �Ķ���� %lf\n",slope[3]);

    printf("\n");

    printf("�Ϲ������� ���� ���� �Էµɰ�� �Ϲ������� ���� ���� ��µɼ� �ֽ��ϴ�.\n\n");
    int inputage, inputgoals, inputassists;
    printf("������ ���̸� �Է��Ͻʽÿ�: ");
    scanf("%d", &inputage);
    printf("�� ���� ���� �� ���� �Է��ϼ���: ");
    scanf("%d", &inputgoals);
    printf("�� ������ ��ý�Ʈ ���� �Է��ϼ���: ");
    scanf("%d", &inputassists);

    EstValue = predict(slope, inputage, inputgoals, inputassists);
    printf("�� ������ ���� ��ġ�� %f �� �� �Դϴ�.\n", EstValue);

    return 0;
}

int loadfile(double age[], double goals[], double assists[], double value[], int *count) {
    // ������ �ҷ����� �Լ�
    FILE *fptr;
    fptr = fopen("data.csv" , "r");

    if (!fptr) {
        printf("������ �� �� �����ϴ�.\n");
        return 0;
    }

    while (fscanf(fptr, "%lf,%lf,%lf,%lf", &age[*count], &goals[*count], &assists[*count], &value[*count]) != EOF) {
        (*count)++;
    }

    fclose(fptr);
}

//����� ��� �Լ�
int invertmatrix(double matrix[4][4], double inverse[4][4]) {
    int i, j, k=0;
    double l;
    double extmat[4][8];

    // ���콺 �ҰŹ� Ȯ�� ��� ����
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            extmat[i][j] = matrix[i][j];
        }
        for (j = 4; j < 8; j++) {
            extmat[i][j] = (i == (j - 4)) ? 1.0 : 0.0;
        }
    }

    // ���콺 �ҰŹ� ����
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (i != j) {
                l = extmat[j][i] / extmat[i][i];
                for (k = 0; k < 8; k++) {
                    extmat[j][k] -= l * extmat[i][k];
                }
            }
        }
    }

    // �ִ밢������ 1�� �����
    for (i = 0; i < 4; i++) {
        double diag = extmat[i][i];
        for (j = 0; j < 8; j++) {
            extmat[i][j] /= diag;
        }
    }

    // ����� ����
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            inverse[i][j] = extmat[i][j + 4];
        }
    }

    return 0;
}

// ���Թ����� ��� �Լ�
int linear_regression(double age[], double goals[], double assists[], double value[], double slope[], int count) {
    double X[count][4];
    double Xt[4][count];
    double y[count];
    double XtXinv[4][4];
    double XtX[4][4] = {0};
    double Xty[4] = {0, 0, 0, 0};
    int i = 0;
    int j = 0;
    int k = 0;

    // X�� y�� �� ����(1.0�� �����(y����))
    for (i = 0; i < count; i++) {
        X[i][0] = 1.0;
        X[i][1] = age[i];
        X[i][2] = goals[i];
        X[i][3] = assists[i];
        y[i] = value[i];
    }

    // X�� ��ġ��� Xt ����
    for (i = 0; i < 4; i++) {
        for (j = 0; j < count; j++) {
            Xt[i][j] = X[j][i];
        }
    }

    // Xt * X ��İ� ���
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            for (k = 0; k < count; k++) {
                XtX[i][j] += Xt[i][k] * X[k][j];
            }
        }
    }

    // XtX�� ����� ���
    invertmatrix(XtX, XtXinv);

    // Xt * y ��İ� ���
    for (i = 0; i < 4; i++) {
        for (k = 0; k < count; k++) {
            Xty[i] += Xt[i][k] * y[k];
        }
    }

    // ���� ȸ�� �Ķ���� ���
    for (i = 0; i < 4; i++) {
        slope[i] = 0.0;
        for (j = 0; j < 4; j++) {
            slope[i] += XtXinv[i][j] * Xty[j];
        }
    }

    return 0;
}

// ���߼���ȸ�� ��� �Լ�
double predict(double slope[], double age, double goals, double assists) {
    return slope[0] + slope[1] * age + slope[2] * goals + slope[3] * assists;
}

