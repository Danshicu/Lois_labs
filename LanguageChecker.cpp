///////////////////////////////////////////////
//
// Лабораторная работа № 1 по дисциплине "Логические основы интеллектуальных систем"
// Выполнена студентами группы 121702 БГУИР Пунинский И.Д. и Промчук Д.В.
// Файл содержит набор функций, определяющий количество различных подформул формулы сокращенного языка логики высказываний
// Дата: 26.04.2023
// Дата: 03.05.2023 v1.1 Добавлена проверка символов, состоящих из 2-ух элементов, обозначающих логические операции
// 
///////////////////////////////////////////////



#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <set>

using namespace std;

struct LogicalThing //структура для парсинга строки на логические элементы 
{
    bool isVariable = 0;
    bool isUnOperator = 0;
    bool isBinOperator = 0;
    bool isInBracket = 0;
    bool isOutBracket = 0;
    LogicalThing()
    {
        this->isVariable = 0;
        this->isUnOperator = 0;
        this->isBinOperator = 0;
        this->isInBracket = 0;
        this->isOutBracket = 0;
    }
};
bool BraсketCheck(string formula) // проверка формулы на соответствие количества открытых и закрытых скобок
{
    if (formula[0] != '(' || formula[formula.size() - 1] != ')')
        return 0;
    int capOfBrakets = 0;
    for (int i = 0; i < formula.size(); i++)
    {
        if (formula[i] == '(')
            capOfBrakets++;
        if (formula[i] == ')')
            capOfBrakets--;
    }
    if (capOfBrakets == 0)
        return 1;
    else
        return 0;
} //Сделал
bool AlphabetCheck(string formula) //проверка формулы на символы принадлежащие алфавиту
{
    vector <char> symbols = { '!', '(', ')', '~', '/', '\\', '.', '-', '>' };
    for (int i = 0; i < formula.size(); i++)
    {
        bool out = 0;
        if (formula[i] >= 65 && formula[i] <= 90) //letters check
            out = 1;
        else if (formula[i] == '0' || formula[i] == '1')    //else if (formula[i] >= 48 && formula[i] <= 57)
            out = 1;
        else
        {
            for (int j = 0; j < symbols.size(); j++)
                if (formula[i] == symbols[j])
                    out = 1;
        }
        if (out == 0)
            return 0;
    }
    return 1;
} //Сделал
bool ParseFormula(string formula, vector <LogicalThing>& ParsedVector) // парсинг формулы на логические элементы
{
    for (int i = 0; i < formula.size(); i++)
    {
        LogicalThing newThing;
        if (formula[i] == '(') //открывающая скобка
        {
            newThing.isInBracket = 1;
            ParsedVector.push_back(newThing);
        }
        else if (formula[i] == ')') //закрывающая скобка
        {
            newThing.isOutBracket = 1;
            ParsedVector.push_back(newThing);
        }
        else if (formula[i] >= 65 && formula[i] <= 90) //переменные
        {
            newThing.isVariable = 1;
            ParsedVector.push_back(newThing);
            /*
            if (i + 1 < formula.size() && (formula[i + 1] >= 49 && formula[i + 1] <= 57))
                while ((formula[i + 1] >= 48 && formula[i + 1] <= 57) && i + 1 < formula.size())
                {
                    i++;
                }
                */
        }
        else if (formula[i] == '0' || formula[i] == '1') //константы
        {
            newThing.isVariable = 1;
            ParsedVector.push_back(newThing);
        }
        else if ((formula[i] == '/' && formula[i + 1] == '\\') || (formula[i] == '\\' && formula[i + 1] == '/') || (formula[i] == '-' && formula[i + 1] == '>')) //бинарные операторы из двух символов
        {
            newThing.isBinOperator = 1;
            ParsedVector.push_back(newThing);
            i += 1;
            continue;
        }
        else if (formula[i] == '~') //бинарные операторы из одного символа
        {
            newThing.isBinOperator = 1;
            ParsedVector.push_back(newThing);
        }
        else if (formula[i] == '!') //унарные операторы из одного символа
        {
            newThing.isUnOperator = 1;
            ParsedVector.push_back(newThing);
        }
        else
            return 0;
    }
    return 1;
}
bool OneOperatorForOneBracketCheck(vector <LogicalThing> ParsedVector, int& index) // проверка на наличие хотябы одного оператора внутри одной пары скобок
{
    int capacityOfOperators = 0;
    bool isFuncTrue = 1;
    for (index = index; index < ParsedVector.size() - 1; index++)
    {
        LogicalThing thing = ParsedVector[index];
        if (thing.isBinOperator == 1 || thing.isUnOperator == 1)
            capacityOfOperators++;
        if (thing.isInBracket == 1)
        {
            index++;
            isFuncTrue = OneOperatorForOneBracketCheck(ParsedVector, index);
        }
        else if (thing.isOutBracket == 1)
            break;
    }
    if (isFuncTrue == 1) {
        if (capacityOfOperators > 1)
            return 0;
        else
            return 1;
    }
    else
        return 0;
} //Сделал
bool DoubleBracketCheck(vector <LogicalThing> ParsedVector) // проверка на наличие только одной пары скобок вокруг одного оператора
{
    int capOfOperators = 0;
    int capOfBrackets = 0;
    for (int i = 0; i < ParsedVector.size(); i++)
    {
        LogicalThing thing = ParsedVector[i];
        if (thing.isBinOperator == 1 || thing.isUnOperator == 1)
        {
            capOfOperators++; 
            continue;
        }
        if (thing.isInBracket == 1)
        {
            capOfBrackets++;
            continue;
        }
    }
    if (capOfBrackets != capOfOperators)
        return 0;
    return 1;
} //Сделал
bool LogicalCheck(string formula) // проверка на расстановку элементов в формуле
{
    vector <LogicalThing> parsedFormula;
    bool parseErr = 0;
    parseErr = !ParseFormula(formula, parsedFormula);
    if (parseErr == 1)
        return 0;
    if (parsedFormula.size() < 3)
        return 0;
    int zero = 0;
    if (!OneOperatorForOneBracketCheck(parsedFormula, zero)) // прверка на один оператор на пару скобок
        return 0;
    if (!DoubleBracketCheck(parsedFormula)) // проверка на дублирующиеся скобки
        return 0;
    for (int i = 0; i < parsedFormula.size(); i++)
    {
        if (i == parsedFormula.size() - 1 && parsedFormula[i].isOutBracket == 1) // проверка на скобку в начеле и конце
            return 1;
        if (parsedFormula[i].isInBracket == 1) //проверка последующих логических элементов после открывающей скобки
        {
            if (!((parsedFormula[i + 1].isInBracket == 1) || (parsedFormula[i + 1].isUnOperator == 1) || (parsedFormula[i + 1].isVariable == 1)))
                return 0;
        }
        else if (parsedFormula[i].isVariable == 1)//проверка последующих логических элементов после переменной
        {
            if (!((parsedFormula[i + 1].isOutBracket == 1) || (parsedFormula[i + 1].isBinOperator == 1)))
                return 0;
        }
        else if (parsedFormula[i].isBinOperator == 1)//проверка последующих логических элементов после бинарного оператора
        {
            if (!((parsedFormula[i + 1].isInBracket == 1) || (parsedFormula[i + 1].isVariable == 1)))
                return 0;
        }
        else if (parsedFormula[i].isUnOperator == 1)//проверка последующих логических элементов после унарного оператора
        {
            if (!((parsedFormula[i + 1].isInBracket == 1) || (parsedFormula[i + 1].isVariable == 1 && parsedFormula[i + 2].isOutBracket == 1)))
                return 0;
        }
        else if (parsedFormula[i].isOutBracket == 1)//проверка последующих логических элементов после закрывающей скобки
        {
            if (!((parsedFormula[i + 1].isOutBracket == 1) || (parsedFormula[i + 1].isBinOperator == 1)))
                return 0;
        }
    }
    return 1;
}
bool VariablesCheck(string formula, set <string>& variables) //проверка переменных на синтаксис
{
    for (int i = 0; i < formula.size(); i++) //перебирая строку находим букву
    { // заменяем букву и все дальнейшие символы соответствующие синтаксису на "_"
        string buffer = "";
        if ((formula[i] == '0' || formula[i] == '1') && i <= formula.size() - 1)
        {
            if (!((formula[i + 1] >= 65 && formula[i + 1] <= 90) || (formula[i + 1] >= 48 && formula[i + 1] <= 57)))
                formula[i] = '_';
            else
                continue;
        }
        if ((formula[i] >= 65 && formula[i] <= 90) && i <= formula.size() - 1)
        {
            if (!((formula[i + 1] >= 65 && formula[i + 1] <= 90) || (formula[i + 1] >= 48 && formula[i + 1] <= 57))) 
            {
                buffer.push_back(formula[i]);
                variables.insert(buffer);
                formula[i] = '_';
            }
            else
                continue;
        }
        /*
        if (((formula[i] >= 65 && formula[i] <= 90) || (formula[i] == '0' || formula[i] == '1')) && (i < formula.size() - 1))
        {
            buffer.push_back(formula[i]);
            formula[i] = '_';
            i++;
            while (((formula[i] >= 65 && formula[i] <= 90) || (formula[i] >= 48 && formula[i] <= 57)) && (i < formula.size() - 1))
            {
                buffer.push_back(formula[i]);
                formula[i] = '_';
                i++;
            }
            variables.insert(buffer);
        }
        */

    }
    bool isThereNoWrongSymbols = 1; //проверяем строку на наличие незамененных символов
    for (int i = 0; i < formula.size(); i++)
        if ((formula[i] >= 65 && formula[i] <= 90) || (formula[i] >= 48 && formula[i] <= 57))
            isThereNoWrongSymbols = 0;
    return isThereNoWrongSymbols;
}

bool CheckSymbolsAroundOperator(string formula, int i)
{
    char previousSymbol = formula[i - 1];
    if (((previousSymbol >= 65 && previousSymbol <= 90) || (previousSymbol >= 48 && previousSymbol <= 57) || (previousSymbol == ')') || (previousSymbol == '('))) {
        char postNextSymbol = formula[i + 2];
        if (((postNextSymbol >= 65 && postNextSymbol <= 90) || (postNextSymbol >= 48 && postNextSymbol <= 57) || (postNextSymbol == ')') || (postNextSymbol == '(')))
            return 1;
    }
    return 0;
} //Сделал

bool CheckOperators(string formula)
{
    for (int i = 0; i < formula.size(); i++)
    {
        char currentChar = formula[i];
        char nextChar = formula[i + 1];
        if (currentChar == '/' && nextChar == '\\')
            if (!CheckSymbolsAroundOperator(formula, i))
            {
                return 0;
            }
            else
                continue;
        if ((currentChar == '\\' && nextChar == '/'))
            if (!CheckSymbolsAroundOperator(formula, i))
            {
                return 0;
            }
            else
                continue;
        if ((currentChar == '-' && nextChar == '>'))
            if (!CheckSymbolsAroundOperator(formula, i))
            {
                return 0;
            }
            else
                continue;
    }
    return 1;
}

// Функция проверки принадлежности языку
bool isFormula(string formula)
{
    if ((formula[0] == '1' || formula[0] == '0') && formula.size() == 1) //проверка формулы состоящей из одной константы
        return 1;
    if ((formula[0] >= 65 && formula[0] <= 90) && formula.size() == 1) //проверка формулы состоящей из одной константы
        return 1;
    /*if (formula[0] >= 65 && formula[0] <= 90)     //проверка формулы состоящей из одной переменной
    {
        int i = 0;
        if (i + 1 < formula.size() && (formula[i + 1] >= 49 && formula[i + 1] <= 57))
            while ((formula[i + 1] >= 48 && formula[i + 1] <= 57) && i + 1 < formula.size())
            {
                i++;
            }
        if (i == formula.size() - 1)
            return 1;
        else
            return 0;
    }
    */
    if (!BraсketCheck(formula))//проверка скобок
        return 0;
    if (!AlphabetCheck(formula))//проверка алфавита
        return 0;
    set <string> unicAtomicFormula;
    if (!VariablesCheck(formula, unicAtomicFormula))//проверка перпеменных на синтаксис
        return 0;
    if (!CheckOperators(formula))
        return 0;
    if (!LogicalCheck(formula)) // проверка на расстановку элементов в формуле
        return 0;
    return 1;
}

void SearchForClosingBracket(string formula, int firstBracket, int& output) //поиск индекса закрывающей скобки
{
    int nesting = 1;
    for (int i = firstBracket + 1; i < formula.size(); i++)
    {
        if (formula[i] == '(')
            nesting++;
        else if ((formula[i] == ')'))
            nesting--;
        if (nesting <= 0)
        {
            output = i;
            break;
        }
    }
} //Сделал
void AddSubformula(string formula, int firstBracket, int secondBracket, set <string>& subFormuls) // добавить подформулу в множество подформул
{
    string substring = "";
    for (int i = firstBracket; i <= secondBracket; i++)
        substring.push_back(formula[i]);
    subFormuls.insert(substring);
} //Сделал

set <string> FindSubformuls(string formula) // поиск подформул
{
    set <string> unicFormula;

    if ((formula[0] == '1' || formula[0] == '0') && formula.size() == 1) //проверка формулы состоящей из одной константы
    {
        unicFormula.insert(formula);
        return unicFormula;
    }

    if (formula[0] >= 65 && formula[0] <= 90)     //если формула начинается с буква
    {
        unicFormula.insert(formula);
        return unicFormula;
    }

    VariablesCheck(formula, unicFormula);// поиск атомарных формул


    int nextBracket = 0;
    for (int i = 0; i < formula.size(); i++) //поиск подформул в строке
    {
        if (formula[i] == '(')
        {
            SearchForClosingBracket(formula, i, nextBracket);
            AddSubformula(formula, i, nextBracket, unicFormula);
        }
    }

    return unicFormula;
}



int main() {
    while (1)
    {
        setlocale(LC_ALL, "ru");
        string formula;
        cout << "Введите формулу логики высказываний: ";
        cin >> formula;
        if (isFormula(formula)) {
            cout << "Формула принадлежит языку" << endl;
            set <string> Subformuls;
            Subformuls = FindSubformuls(formula);
            cout << "Количество уникальных подформул в заданной формуле: ";
            cout << Subformuls.size() << endl;
            cout << "Уникальные подформулы заданной формулы:" << endl;
            for (std::set<string>::iterator i = Subformuls.begin(); i != Subformuls.end(); ++i)
            {
                cout << *i << endl;
            }
        }
        else {
            cout << "Формула не принадлежит языку" << endl;
        }
    }
    return 0;
}


