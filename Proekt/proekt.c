#include <stdio.h>
#include <string.h>

// Максимален капацитет на опашката и максимална дължина на имената
#define MAX_SIZE 100
#define MAX_NAME 50

// Дефиниране на потребителски тип данни, обединяващ информацията за един пациент
typedef struct {
    char name[MAX_NAME];
    int age;
    int id;
} Patient;

// Инициализация на масива (опашката) и помощните променливи за състоянието
Patient queue[MAX_SIZE];
int count = 0;
int next_id = 1;

// Предварително деклариране на функциите, за да са видими за main
void addPatient();
void callNextPatient();
void showQueue();
void searchPatient();
void showStats();

int main() {
    int choice;

    // Основен цикъл на програмата, който държи менюто активно
    do {
        printf("\n================================\n");
        printf("   HOSPITAL INFORMATION SYSTEM\n");
        printf("================================\n");
        printf("1. Register new patient\n");
        printf("2. Call next patient to the office\n");
        printf("3. View waiting patients\n");
        printf("4. Search patient by name\n");
        printf("5. Statistics (average age)\n");
        printf("0. Exit\n");
        printf("Choose an option: ");
        
        // Валидация дали потребителят е въвел число, а не текст
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            // Изчистване на буфера при грешен вход, за да се избегне безкраен цикъл
            while(getchar() != '\n');
            continue;
        }

        // Разпределяне на логиката спрямо избора на потребителя
        switch (choice) {
            case 1: addPatient(); break;
            case 2: callNextPatient(); break;
            case 3: showQueue(); break;
            case 4: searchPatient(); break;
            case 5: showStats(); break;
            case 0: printf("Goodbye!\n"); break;
            default: printf("Invalid choice!\n");
        }
    } while (choice != 0);

    return 0;
}

// Функция за добавяне на нов запис в края на опашката
void addPatient() {
    // Проверка за препълване на масива
    if (count >= MAX_SIZE) {
        printf("\nError: Maximum capacity reached!\n");
        return;
    }

    printf("\n--- New registration ---\n");
    printf("First and last name: ");
    
    // Премахване на остатъчния символ за нов ред, за да работи fgets коректно
    getchar();
    
    // Четене на целия ред, включително интервалите между имената
    fgets(queue[count].name, MAX_NAME, stdin);
    
    // Замяна на символа за нов ред с нулев терминатор за край на низа
    queue[count].name[strcspn(queue[count].name, "\n")] = 0;

    // Цикъл за гарантиране на въвеждане на валидна възраст
    int validAge = 0;
    do {
        printf("Age: ");
        if (scanf("%d", &queue[count].age) == 1) {
            // Проверка дали годините са в реалистични граници
            if (queue[count].age > 0 && queue[count].age < 120) {
                validAge = 1;
            } else {
                printf("Please enter a realistic age (between 1 and 120)!\n");
            }
        } else {
            // Съобщение при въвеждане на букви вместо цифри
            printf("Please enter a number, not text!\n");
            while(getchar() != '\n');
        }
    } while (!validAge);
    
    // Присвояване на уникален номер и увеличаване на общия брояч
    queue[count].id = next_id++;
    count++;
    printf("Patient successfully added with ID: %d\n", queue[count-1].id);
}

// Функция за премахване на първия пациент и пренареждане на останалите
void callNextPatient() {
    // Проверка дали опашката е празна
    if (count == 0) {
        printf("\nThere are no waiting patients.\n");
        return;
    }

    // Визуализиране на данните на първия пациент в списъка
    printf("\n>>> To the office: [%d] %s, %d years old\n",
           queue[0].id, queue[0].name, queue[0].age);

    // Изместване на всеки следващ елемент с една позиция напред
    for (int i = 0; i < count - 1; i++) {
        queue[i] = queue[i + 1];
    }

    // Намаляване на общия брой чакащи
    count--;
}

// Функция за обхождане и принтиране на всички текущи записи
void showQueue() {
    if (count == 0) {
        printf("\nThe queue is empty.\n");
        return;
    }
    printf("\n--- Waiting list (%d people) ---\n", count);
    for (int i = 0; i < count; i++) {
        printf("%d. ID: %d | %s (%d years old)\n",
               i + 1, queue[i].id, queue[i].name, queue[i].age);
    }
}

// Функция за линейно търсене на пациент по име
void searchPatient() {
    char target[MAX_NAME];
    printf("\nEnter first and last name to search: ");
    
    // Подготовка на буфера и четене на търсеното име
    getchar();
    fgets(target, MAX_NAME, stdin);
    target[strcspn(target, "\n")] = 0;

    int found = 0;
    // Обхождане на масива и сравняване на низовете
    for (int i = 0; i < count; i++) {
        if (strcmp(queue[i].name, target) == 0) {
            printf("Found: %s is at position %d in the queue.\n", target, i + 1);
            found = 1;
            break;
        }
    }
    if (!found)
        printf("Patient with name '%s' was not found.\n", target);
}

// Функция за изчисляване на средно аритметично на възрастта
void showStats() {
    if (count == 0) {
        printf("\nNo data available for statistics.\n");
        return;
    }

    int sum = 0;
    // Натрупване на сумата от годините на всички пациенти
    for (int i = 0; i < count; i++) {
        sum += queue[i].age;
    }
    // Преобразуване към дробно число за точно деление
    printf("\nAverage age of waiting patients: %.2f years.\n",
           (float)sum / count);
}
