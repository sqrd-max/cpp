#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <string>

// Перечисление для пола
enum class Gender { Male, Female };

// Структура Person для представления человека
struct Person {
    std::string name;  // Строковое поле для имени
    int age;           // Целочисленное поле для возраста
    Gender gender;     // Пол человека (используется перечисление)

    // Конструкторы
    Person() = default;  // Конструктор по умолчанию
    Person(const std::string& n, int a, Gender g) : name(n), age(a), gender(g) {}
    Person(const Person& other) = default;                 // Конструктор копирования
    Person(Person&& other) noexcept = default;             // Конструктор перемещения
    Person& operator=(const Person& other) = default;      // Оператор присваивания
    ~Person() = default;                                   // Деструктор
};

// Перегрузка оператора вывода для сериализации объекта Person
std::ostream& operator<<(std::ostream& os, const Person& person) {
    os << person.name << ',' << person.age << ',';
    switch (person.gender) {
        case Gender::Male:
            os << 'M';
            break;
        case Gender::Female:
            os << 'F';
            break;
    }
    os << '\n';
    return os;
}

// Перегрузка оператора ввода для десериализации объекта Person
std::istream& operator>>(std::istream& is, Person& person) {
    // Считываем строку целиком
    std::string line;
    std::getline(is, line);

    // Создаем поток для строки для удобства извлечения значений
    std::istringstream lineStream(line);

    // Извлекаем значения из строки
    std::getline(lineStream, person.name, ',');
    lineStream >> person.age;
    lineStream.ignore();  // Пропускаем запятую
    char genderChar;
    lineStream.get(genderChar);
    lineStream.ignore();  // Пропускаем запятую

    // Преобразуем символ пола в значение перечисления
    switch (genderChar) {
        case 'M':
            person.gender = Gender::Male;
            break;
        case 'F':
            person.gender = Gender::Female;
            break;
    }

    return is;
}

// Обобщенная функция для сериализации вектора в поток
template <typename T>
bool serializeSpan(std::ostream& os, const std::vector<T>& data) {
    for (const auto& item : data) {
        os << item;
    }
    return true;
}

// Обобщенная функция для десериализации потока в вектор
template <typename T>
std::vector<T> deserializeStream(std::istream& is) {
    std::vector<T> result;
    T item;
    while (is >> item) {
        result.push_back(item);
    }
    return result;
}

int main() {
    // Создаем файл для записи данных
    std::ofstream outFile("data.csv");

    // Создаем вектор объектов Person с случайными данными
    std::vector<Person> persons;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> ageDistribution(18, 60);

    for (int i = 0; i < 5; ++i) {
        Person person("Person" + std::to_string(i), ageDistribution(gen),
                      (i % 2 == 0) ? Gender::Male : Gender::Female);
        persons.push_back(person);
    }

    // Сериализуем данные в файл
    if (serializeSpan(outFile, persons)) {
        std::cout << "Data successfully serialized.\n";
    } else {
        std::cerr << "Error: Unable to serialize data.\n";
        return 1;
    }
    outFile.close();

    // Создаем файл для чтения данных
    std::ifstream inFile("data.csv");

    // Десериализуем данные из файла
    auto deserializedPersons = deserializeStream<Person>(inFile);
    inFile.close();

    // Модификация данных
    if (!deserializedPersons.empty()) {
        deserializedPersons[0].name = "ModifiedName";
        Person newPerson("NewPerson", 25, Gender::Female);
        deserializedPersons.push_back(newPerson);
    }

    // Создаем файл для записи измененных данных
    std::ofstream modifiedFile("modified_data.csv");
    if (serializeSpan(modifiedFile, deserializedPersons)) {
        std::cout << "Modified data successfully serialized.\n";
    } else {
        std::cerr << "Error: Unable to serialize modified data.\n";
        return 1;
    }
    modifiedFile.close();

    return 0;
}
