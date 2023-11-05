#include <stdio.h>
#include <stdlib.h>

int student_Number = 1; // 학생 수 (초기 값을 1로 정하는 이유는 내 자신을 포함하기 위해서다.)
int* students; // 숫자를 저장할 배열 
int my_Score; // 내 점수를 저장할 변수
int* grade_Number; // 학점 별 학생 수를 저장할 배열
int cutLine_A; // A 학점 커트라인 점수
int cutLine_B; // B 학점 커트라인 점수
char my_Grade; // 내 학점을 저장할 변수

// 함수 선언
void init(); // 파일 입출력으로 txt 파일을 읽어오는 함수
void run(); // 알고리즘을 실행해주는 함수
int getScore(int A[], int left, int right, int k); // 원하는 학생의 원점수를 가져오는 함수 
char evalGrade(int score); // 본인의 원점수가 어떤 학점에 해당하는 지 알려주는 함수
int* calculator(int student_Number); // 총 학생 수를 학점 퍼센트(30% 50% 20%) 나누는 함수
void swap(int* a, int* b); // 배열의 두 원소 교환 함수
void end(); // 메모리 할당을 해제 함수

int main() {
    init(); // init 함수 실행
    run(); // run 함수 실행
    return 0;
    // ================================================ 조건 만족 ================================================ 
    // 5. main 함수에는 init() 함수와 run() 함수만 존재할 것
    // ========================================================================================================= 
}

// 파일 입출력으로 txt 파일을 읽어오는 함수
void init() {
    FILE* file = fopen("grade.txt", "r"); // grade.txt 파일을 읽는다.
    if (file == NULL) { // 만약 파일이 없다면,
        printf("파일을 열 수 없습니다.\n");
        exit(1); // 종료한다.
    }

    int number;
    while (fscanf(file, "%d", &number) == 1) { // 파일에서 학생 수를 센다.
        student_Number++;
    }

    fseek(file, 0, SEEK_SET); // 파일을 다시 처음으로 되돌린다.

    students = (int*)malloc(sizeof(int) * student_Number); // 숫자를 저장할 배열 동적 할당
    if (students == NULL) {
        printf("메모리 할당 실패\n");
        fclose(file);
        exit(1);
    }

    students[0] = -999; // run 함수에서 my_Score를 입력받아 students 배열 0번째에 넣기 위해 임시로 -999 값을 넣는다.

    for (int i = 1; i < student_Number; i++) { // grade.txt 파일에서 점수를 가져와서 배열 students에 넣는다.
        fscanf(file, "%d", &students[i]);
    }

    fclose(file); // 파일 닫기
}

// 알고리즘을 실행해주는 함수로, 최종적으로 출력형태가 나올 수 있도록 한다
void run() {
    printf("201901551 김정목\n");
    printf("본인의 원점수를 입력해주세요 : ");
    scanf("%d", &my_Score);

    students[0] = my_Score; // students 배열 0번째에 my_Score 값을 넣는다.

    printf("총 학생 수 (본인 포함) : %d명\n", student_Number);

    calculator(student_Number); // calculator 함수 실행
    printf("학점 당 학생 수 | A : %d명, B : %d명, C : %d명\n", grade_Number[0], grade_Number[1], grade_Number[2]);

    cutLine_A = getScore(students, 0, student_Number - 1, grade_Number[0]); // getScore 함수 실행
    cutLine_B = getScore(students, 0, student_Number - 1, grade_Number[0] + grade_Number[1]); // getScore 함수 실행


    printf("A학점 마지막 석차 학생의 원점수 : %d\n", cutLine_A);
    printf("B학점 마지막 석차 학생의 원점수 : %d\n", cutLine_B);

    my_Grade = evalGrade(my_Score); // evelGrade 함수를 통해 내 학점 저장
    printf("김정목 학생은 현재 %c학점에 해당하는 석차에 있습니다\n", my_Grade);

    end(); // end 함수 실행
    // ================================================ 조건 만족 ================================================ 
    // 6. main 함수의 202301234 홍길동 부분과 홍길동 학생은~ 부분은 본인의 학번/이름으로 작성할 것
    // ========================================================================================================= 
}

// 원하는 학생의 원점수를 가져오는 함수 (ex. 10등 학생의 점수 가져오기)
int getScore(int A[], int left, int right, int k) {
    if (left == right) { // 만약 left와 right가 같다면, 
        return A[left]; // A[left] 반환
    }

    int pivotIndex = (left + right) / 2; // Pivot index 값 선택 (그냥 left와 right 중간 값으로 정했습니다!!)

    swap(&A[pivotIndex], &A[left]); // Pivot 위치 업데이트
    pivotIndex = left; // Pivot 인덱스 업데이트
    int pivotValue = A[pivotIndex]; // Pivot 값 업데이트
    int i = left + 1; // 왼쪽부터 시작
    int j = right;    // 오른쪽부터 시작

    // Pivot을 기준으로 작은 값과 큰 값으로 배열을 분할 (left와 right로 양쪽으로 비교한다.)
    while (1) {
        while (i <= j && A[i] > pivotValue) { // pivotValue 보다 작은 값을 찾을 때까지 왼쪽에서 오른쪽으로 이동한다.
            i++;
        }
        while (i <= j && A[j] < pivotValue) { // pivotValue 보다 큰 값을 찾을 때까지 오른쪽에서 왼쪽으로 이동한다.
            j--;
        }
        if (i <= j) {
            swap(&A[i], &A[j]); // 위의 두 while 루프에서 찾은 작은 값과 큰 값을 서로 교환한다.
            i++; // 왼쪽 인덱스 증가
            j--; // 오른쪽 인덱스 감소
        }
        else { // i와 j가 교차하면 분할이 완료되었으므로 루프를 종료한다.
            break;
        }
    }

    swap(&A[pivotIndex], &A[j]); // Pivot 위치 업데이트
    pivotIndex = j; // Pivot 인덱스 업데이트

    int L = (pivotIndex - 1) - left + 1; // L = Large group 크기

    if (k <= L) { // Large group에서 찾기
        return getScore(A, left, pivotIndex - 1, k);
    }
    else if (k == L + 1) { // Pivot = k번째 작은 숫자
        return A[pivotIndex];
    }
    else { // Small group에서 찾기
        return getScore(A, pivotIndex + 1, right, k - L - 1);
    }
    // ================================================ 조건 만족 ================================================ 
    // 1. 정렬을 사용하지 않고 구현할 것 
    // 2. 분할정복법을 활용할 것
    // ========================================================================================================= 
}

// 본인의 원점수가 어떤 학점에 해당하는 지 알려주는 함수
char evalGrade(int score) {
    if (score >= cutLine_A) {
        return 'A';
    }
    else if (score >= cutLine_B) {
        return 'B';
    }
    else {
        return 'C';
    }
}

// 총 학생 수를 학점 퍼센트(30% 50% 20%) 나누는 함수
int* calculator(int student_Number) {
    double total_students = (double)student_Number; 

    // 각 학점에 대한 비율 (A: 30%, B: 50%, C: 20%)
    double a_ratio = 0.3;
    double b_ratio = 0.5;
    double c_ratio = 0.2;

    // 각 학점에 대한 인원 계산
    int a_number = (int)(total_students * a_ratio);
    int b_number = (int)(total_students * b_ratio);
    int c_number; 

    // A 학점 동점자 계산
    while(1) { 
        int a_Last = getScore(students, 0, student_Number - 1, a_number); // A 학점 커트라인 점수 
        int a_count = 0; // 동점자 수 카운트 변수
        
        for(int i = a_number-1; i >= 1; i--) { // 반복문을 통해 A 학점 커트라인 점수와 a_number보다 작은 인덱스 값을 갖는 점수들과 비교한다.
            if(a_Last == getScore(students, 0, student_Number - 1, i)) { // 만약 점수가 동일하다면,
                a_count++; // a_count++ 한다.
            }
            else { // 동일하지 않다면,
                break; // for문 종료
            }
        }

        if(a_count != 0) { // 동점자 수가 0이 아니라면,
            a_number = a_number - a_count - 1;
            b_number = b_number + a_count;
            break; // while문 종료
        } else { // 동점자 수가 0이라면,
            if(a_Last == getScore(students, 0, student_Number - 1, a_number + 1)) { // A 학점 커트라인 점수와 a_number + 1 인덱스 값을 갖는 점수와 비교한다. 
                a_number = a_number - 1;
                b_number = b_number + 1;
            } 
            break; // while문 종료
        }
    }

    // B 학점 동점자 계산
    while(1) {
        int b_Last = getScore(students, 0, student_Number - 1, a_number + b_number); // B 학점 커트라인 점수 
        int b_count = 0; // 동점자 수 카운트 변수

        for(int i = a_number + b_number - 1; i >= 1; i--) { // 반복문을 통해 B 학점 커트라인 점수와 a+number + b_number보다 작은 인덱스 값을 갖는 점수들과 비교한다.
            if(b_Last == getScore(students, 0, student_Number - 1, i)) { // 만약 점수가 동일하다면,
                b_count++; // b_count++ 한다.
            }
            else { // 동일하지 않다면,
                break; // for문 종료
            }
        }

        if(b_count != 0) { // 동점자 수가 0이 아니라면,
            b_number = b_number - b_count - 1;
            break; // while문 종료
        } else { // 동점자 수가 0이라면,
            if(b_Last == getScore(students, 0, student_Number - 1, a_number + b_number + 1)) { // B 학점 커트라인 점수와 a_number + b_number+1 인덱스 값을 갖는 점수와 비교한다. 
                b_number = b_number - 1;
            } 
            break; // while문 종료
        }
    }

    c_number = student_Number - (a_number + b_number); // 남은 인원을 C 학점으로 할당

    grade_Number = (int*)malloc(3 * sizeof(int)); // 결과를 grade_Number 배열에 저장
    if (grade_Number == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }

    // 배열에 학점별 인원수 저장
    grade_Number[0] = a_number;
    grade_Number[1] = b_number;
    grade_Number[2] = c_number;

    return grade_Number;
    // ================================================ 조건 만족 ================================================ 
    // 3. 성적은 상대평가로, +, -, 0 등은 반영하지 않고 A, B, C만 존재하며, 비율은 각각 30%, 50%, 20%임. 
    // (학점에 대한 인원은 반올림이 아닌 내림으로 계산하여 10.8명이 30%라면 10명이 A학점을 받는다, 만약 내림으로 계산하여 남는 인원이 발생하는 경우 해당 인원은 C학점의 인원으로 계산한다)
    // 4. 동점자가 발생하면 등급은 내려감 (10등까지 A일 때, 공동 10등이 발생하면 공동 10등은 B로 계산)
    // ========================================================================================================= 
}

// 배열의 두 원소 교환 함수
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 메모리 할당을 해제 함수
void end() {
    free(students); // students 배열 할당 해제
    free(grade_Number); // grade_Number 배열 할당 해제
}

// ================================================ 조건 만족 ================================================ 
// 7. 작성한 소스코드에 대한 주석을 작성할 것
// ========================================================================================================= 
