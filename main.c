#include <stdio.h>
#include <stdlib.h>

 // 데이터 불러오기 함수
int loadfile(double age[], double goals[], double assists[], double value[], int *count);
//역행렬 계산 함수
int invertmatrix(double matrix[4][4], double inverse[4][4]);
// 정규방정식 계산 함수
int linear_regression(double age[], double goals[], double assists[], double value[], double slope[], int count);
// 다중선형회귀 계산 함수
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

    printf("이 프로그램은 축구 공격수의 나이,골,어시스트 를 입력하면 예상 시장가치를 구해줍니다\n\n");

    printf("선형 회귀 모델 파라미터 출력 ");
    printf("\n상수항의 모델 파라미터%lf\n",slope[0]);
    printf("나이의 모델 파라미터 %lf\n",slope[1]);
    printf("골 수의 모델 파라미터 %lf\n",slope[2]);
    printf("어시스트 수의 모델 파라미터 %lf\n",slope[3]);

    printf("\n");

    printf("일반적이지 않은 값이 입력될경우 일반적이지 않은 값이 출력될수 있습니다.\n\n");
    int inputage, inputgoals, inputassists;
    printf("선수의 나이를 입력하십시오: ");
    scanf("%d", &inputage);
    printf("한 시즌에 넣은 골 수를 입력하세요: ");
    scanf("%d", &inputgoals);
    printf("한 시즌의 어시스트 수를 입력하세요: ");
    scanf("%d", &inputassists);

    EstValue = predict(slope, inputage, inputgoals, inputassists);
    printf("이 선수의 예상 가치는 %f 억 원 입니다.\n", EstValue);

    return 0;
}

int loadfile(double age[], double goals[], double assists[], double value[], int *count) {
    // 데이터 불러오기 함수
    FILE *fptr;
    fptr = fopen("data.csv" , "r");

    if (!fptr) {
        printf("파일을 열 수 없습니다.\n");
        return 0;
    }

    while (fscanf(fptr, "%lf,%lf,%lf,%lf", &age[*count], &goals[*count], &assists[*count], &value[*count]) != EOF) {
        (*count)++;
    }

    fclose(fptr);
}

//역행렬 계산 함수
int invertmatrix(double matrix[4][4], double inverse[4][4]) {
    int i, j, k=0;
    double l;
    double extmat[4][8];

    // 가우스 소거법 확장 행렬 생성
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            extmat[i][j] = matrix[i][j];
        }
        for (j = 4; j < 8; j++) {
            extmat[i][j] = (i == (j - 4)) ? 1.0 : 0.0;
        }
    }

    // 가우스 소거법 적용
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

    // 주대각성분을 1로 만든다
    for (i = 0; i < 4; i++) {
        double diag = extmat[i][i];
        for (j = 0; j < 8; j++) {
            extmat[i][j] /= diag;
        }
    }

    // 역행렬 추출
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            inverse[i][j] = extmat[i][j + 4];
        }
    }

    return 0;
}

// 정규방정식 계산 함수
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

    // X와 y의 값 대입(1.0은 상수항(y절편))
    for (i = 0; i < count; i++) {
        X[i][0] = 1.0;
        X[i][1] = age[i];
        X[i][2] = goals[i];
        X[i][3] = assists[i];
        y[i] = value[i];
    }

    // X의 전치행렬 Xt 생성
    for (i = 0; i < 4; i++) {
        for (j = 0; j < count; j++) {
            Xt[i][j] = X[j][i];
        }
    }

    // Xt * X 행렬곱 계산
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            for (k = 0; k < count; k++) {
                XtX[i][j] += Xt[i][k] * X[k][j];
            }
        }
    }

    // XtX의 역행렬 계산
    invertmatrix(XtX, XtXinv);

    // Xt * y 행렬곱 계산
    for (i = 0; i < 4; i++) {
        for (k = 0; k < count; k++) {
            Xty[i] += Xt[i][k] * y[k];
        }
    }

    // 선형 회귀 파라미터 계산
    for (i = 0; i < 4; i++) {
        slope[i] = 0.0;
        for (j = 0; j < 4; j++) {
            slope[i] += XtXinv[i][j] * Xty[j];
        }
    }

    return 0;
}

// 다중선형회귀 계산 함수
double predict(double slope[], double age, double goals, double assists) {
    return slope[0] + slope[1] * age + slope[2] * goals + slope[3] * assists;
}

