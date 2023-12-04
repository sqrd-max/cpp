#include <iostream>
#include <array>
#include <optional>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <random>
#include <string>
#include <span>

// Enum для Gender
enum class Gender { Male, Female };

// Структура данных
struct Person {
    std::array<char, 50> name;  // Строковое поле (C-string)
    int age;                   // Целочисленное поле
    Gender gender;             // Enum поле
};


// Реализация шаблонной функции serializePerson для типа Person
std::string serializePerson(const Person& person) {
    std::ostringstream oss;
    oss << person.name.data() << ',' << person.age << ',';

    // Сериализация enum Gender
    switch (person.gender) {
    case Gender::Male:
        oss << 'M';
        break;
    case Gender::Female:
        oss << 'F';
        break;
        // Добавьте обработку других значений enum при необходимости
    }

    // Дополнительные поля, если они есть
    // oss << ',' << person.additionalField;

    // Завершаем строку
    oss << '\n';

    return oss.str();
}



// Функция для десериализации строки
std::optional<std::array<char, 50>> deserializeString(std::istream& is) {
    std::array<char, 50> buffer;
    is.get(buffer.data(), buffer.size(), ',');
    is.ignore();  // Пропускаем запятую
    return buffer;
}

// Функция для десериализации целочисленного значения
std::optional<int> deserializeInt(std::istream& is) {
    std::string valueStr;
    std::getline(is, valueStr, ',');
    try {
        return std::stoi(valueStr);
    } catch (const std::invalid_argument& e) {
        // Не удалось преобразовать в число
        return std::nullopt;
    }
}

// Функция для десериализации enum Gender
std::optional<Gender> deserializeGender(std::istream& is) {
    char genderChar;
    is.get(genderChar);
    is.ignore();  // Пропускаем запятую
    switch (genderChar) {
        case 'M':
            return Gender::Male;
        case 'F':
            return Gender::Female;
        default:
            // Недопустимое значение для Gender
            return std::nullopt;
    }
}

// Функция для десериализации всего объекта Person
std::optional<Person> deserializePerson(std::istream& is) {
    Person person;
    
    auto nameResult = deserializeString(is);
    if (!nameResult) {
        return std::nullopt;  // Ошибка при десериализации строки
    }
    std::copy(nameResult->begin(), nameResult->end(), person.name.begin());
    
    auto ageResult = deserializeInt(is);
    if (!ageResult) {
        return std::nullopt;  // Ошибка при десериализации целочисленного значения
    }
    person.age = *ageResult;
    
    auto genderResult = deserializeGender(is);
    if (!genderResult) {
        return std::nullopt;  // Ошибка при десериализации enum Gender
    }
    person.gender = *genderResult;

    return person;
}

// Структура для результатов десериализации
struct DeserializationResult {
    bool success;
    std::optional<Person> value;
};

// Функция для десериализации всего объекта Person
DeserializationResult deserialize(std::istream& is) {
    Person result;
    is >> std::ws;  // Пропускаем пробелы и переводы строк перед чтением
    auto personResult = deserializePerson(is);
    if (!personResult) {
        return { false, std::nullopt };  // Ошибка при десериализации объекта Person
    }
    return { true, *personResult };
}
// Общий интерфейс для функции serializePerson
std::string serializePerson(const Person& person);

// Реализация шаблонной функции serializeSpan для типа Person
template <typename Iterator>
bool serializeSpan(std::ostream& os, Iterator begin, Iterator end) {
    for (auto it = begin; it != end; ++it) {
        // Проверка наличия запятых в строковом поле перед сериализацией
        if (std::string(it->name.data()).find(',') != std::string::npos) {
            return false;  // Сигнализируем об ошибке
        }

        os << serializePerson(*it);
        os << '\n';  // Разделяем объекты переходом на новую строку
    }
    return true;
}



// Функция для десериализации std::istream в std::vector объектов структуры
template <std::size_t N>
std::vector<Person> deserializeStream(std::istream& is) {
    std::vector<Person> persons;
    std::string line;
    while (std::getline(is, line)) {
        std::istringstream lineStream(line);
        auto personResult = deserializePerson(lineStream);
        if (personResult) {
            persons.push_back(*personResult);
        }
        // Возможно, здесь стоит добавить проверку на ошибки десериализации
    }
    return persons;
}

int main() {
    // Создаем файл и записываем в него случайные данные
    std::ofstream outFile("data.csv");
    std::vector<Person> persons;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> ageDistribution(18, 60);

    for (int i = 0; i < 5; ++i) {
        Person person;
        // Генерируем случайные данные для тестов
        snprintf(person.name.data(), person.name.size(), "Person%d", i);
        person.age = ageDistribution(gen);
        person.gender = (i % 2 == 0) ? Gender::Male : Gender::Female;

        persons.push_back(person);
    }

    // Сериализуем данные в файл
    if (serializeSpan(outFile, persons.begin(), persons.end())) {
        std::cout << "Data successfully serialized.\n";
    } else {
        std::cerr << "Error: Unable to serialize data due to commas in string fields.\n";
        return 1;
    }
    outFile.close();

    // Десериализуем данные из файла
    std::ifstream inFile("data.csv");
    auto deserializedPersons = deserializeStream<5>(inFile);
    inFile.close();

    // Модификация данных
    if (!deserializedPersons.empty()) {
        // Изменяем поле name первого объекта
        snprintf(deserializedPersons[0].name.data(), deserializedPersons[0].name.size(), "ModifiedName");

        // Добавляем новый объект
        Person newPerson;
        snprintf(newPerson.name.data(), newPerson.name.size(), "NewPerson");
        newPerson.age = 25;
        newPerson.gender = Gender::Female;
        deserializedPersons.push_back(newPerson);
    }

    // Сериализуем измененные данные обратно в файл
    std::ofstream modifiedFile("modified_data.csv");
    if (serializeSpan(modifiedFile, deserializedPersons.begin(), deserializedPersons.end())) {
        std::cout << "Modified data successfully serialized.\n";
    } else {
        std::cerr << "Error: Unable to serialize modified data due to commas in string fields.\n";
        return 1;
    }
    modifiedFile.close();

    return 0;
}