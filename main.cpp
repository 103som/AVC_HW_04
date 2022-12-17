#include <algorithm>
#include <fstream>
#include <iostream>
#include <pthread.h>

// Мьютекс и текущая позиция в бибилиотеке
pthread_mutex_t mutex;
int curN = 0;

// Текущий поток
int cnt = 0;

// Библиотека
const int n = 10;
const int m = 20;
int library[n][m];

std::ifstream fin; // Поток для чтения.
std::ofstream fout; // Поток для вывода.

// Метод сортировки книг по каталогам
void *sortBooks(void *param)
{
  // Блок мьютекса
  pthread_mutex_lock(&mutex);
  // Соритровка книжек
  std::sort(std::begin(library[curN]), std::end(library[curN]));
  std::cout << "Shelve num " << curN << " was sorted. \n";
  fout << "Shelve num " << curN << " was sorted. \n";
  for (int i = 0; i < m; ++i) {
    std::cout << library[curN][i] << " ";
  }
  std::cout << '\n';
  curN++;
  pthread_mutex_unlock(&mutex);// unlock thread
}

int main(int argc, char **argv) {
  // Рандомное число, argc == 2 рандом
  int randNum = 1;
  if (argc == 2) {
    char *arg = argv[1];
    int randSeed = atoi(arg);
    srand(randSeed);
    randNum = rand() % 30;
    // Считываение из файла
  } else if (argc == 3) {
    fin.open(argv[1]);
    fout.open(argv[2]);
        
    if (!fin.is_open() || !fout.is_open()) {
        std::cout << "Проблемы с открытием файла.";
        return 0;
    }
    
    fin >> randNum;
  } else {
    std::cout << "Enter num for books: " << '\n';
    std::cin >> randNum;
    
    fout.open("output.txt");
  }

  // Номер каталога
  int libRand = 1000;
  // Заполнение библиотеке
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < m; ++j) {
      library[i][j] = libRand;
      libRand -= randNum;
    }
  }

  // Поток студентов
  pthread_t students[n];

  // Инициализация мьютекса
  pthread_mutex_init(&mutex, 0);

  // Портфель задач. Каждый студент разбирает своё задание ( то есть сортировку )
  for (int i = 0; i < n; ++i) {
    pthread_create(&students[i], 0, sortBooks, (void *) cnt);
    std::cout << "Student  " << i << " started" << '\n';
    fout << "Student  " << i << " started" << '\n';
  }

  // Выполнение параллелизации
  for (int i = 0; i < n; ++i) {
    pthread_join(students[i], 0);
    std::cout << "Student " << i << "  finished" << '\n';
    fout << "Student " << i << "  finished" << '\n';
  }

  pthread_mutex_destroy(&mutex);
  return 0;
}
