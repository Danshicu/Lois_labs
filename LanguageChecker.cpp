///////////////////////////////////////////////
//
// ������������ ������ � 1 �� ���������� "���������� ������ ���������������� ������"
// ��������� ���������� ������ 121702 ����� ��������� �.�. � ������� �.�.
// ���� �������� ����� �������, ������������ ���������� ��������� ��������� ������� ������������ ����� ������ ������������
// ����: 26.04.2023
// ����: 03.05.2023 v1.1 ��������� �������� ��������, ��������� �� 2-�� ���������, ������������ ���������� ��������
// 
///////////////////////////////////////////////



#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <set>

using namespace std;

struct LogicalThing //��������� ��� �������� ������ �� ���������� �������� 
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
bool Bra�ketCheck(string formula) // �������� ������� �� ������������ ���������� �������� � �������� ������
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
} //������
bool AlphabetCheck(string formula) //�������� ������� �� ������� ������������� ��������
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
} //������
bool ParseFormula(string formula, vector <LogicalThing>& ParsedVector) // ������� ������� �� ���������� ��������
{
    for (int i = 0; i < formula.size(); i++)
    {
        LogicalThing newThing;
        if (formula[i] == '(') //����������� ������
        {
            newThing.isInBracket = 1;
            ParsedVector.push_back(newThing);
        }
        else if (formula[i] == ')') //����������� ������
        {
            newThing.isOutBracket = 1;
            ParsedVector.push_back(newThing);
        }
        else if (formula[i] >= 65 && formula[i] <= 90) //����������
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
        else if (formula[i] == '0' || formula[i] == '1') //���������
        {
            newThing.isVariable = 1;
            ParsedVector.push_back(newThing);
        }
        else if ((formula[i] == '/' && formula[i + 1] == '\\') || (formula[i] == '\\' && formula[i + 1] == '/') || (formula[i] == '-' && formula[i + 1] == '>')) //�������� ��������� �� ���� ��������
        {
            newThing.isBinOperator = 1;
            ParsedVector.push_back(newThing);
            i += 1;
            continue;
        }
        else if (formula[i] == '~') //�������� ��������� �� ������ �������
        {
            newThing.isBinOperator = 1;
            ParsedVector.push_back(newThing);
        }
        else if (formula[i] == '!') //������� ��������� �� ������ �������
        {
            newThing.isUnOperator = 1;
            ParsedVector.push_back(newThing);
        }
        else
            return 0;
    }
    return 1;
}
bool OneOperatorForOneBracketCheck(vector <LogicalThing> ParsedVector, int& index) // �������� �� ������� ������ ������ ��������� ������ ����� ���� ������
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
} //������
bool DoubleBracketCheck(vector <LogicalThing> ParsedVector) // �������� �� ������� ������ ����� ���� ������ ������ ������ ���������
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
} //������
bool LogicalCheck(string formula) // �������� �� ����������� ��������� � �������
{
    vector <LogicalThing> parsedFormula;
    bool parseErr = 0;
    parseErr = !ParseFormula(formula, parsedFormula);
    if (parseErr == 1)
        return 0;
    if (parsedFormula.size() < 3)
        return 0;
    int zero = 0;
    if (!OneOperatorForOneBracketCheck(parsedFormula, zero)) // ������� �� ���� �������� �� ���� ������
        return 0;
    if (!DoubleBracketCheck(parsedFormula)) // �������� �� ������������� ������
        return 0;
    for (int i = 0; i < parsedFormula.size(); i++)
    {
        if (i == parsedFormula.size() - 1 && parsedFormula[i].isOutBracket == 1) // �������� �� ������ � ������ � �����
            return 1;
        if (parsedFormula[i].isInBracket == 1) //�������� ����������� ���������� ��������� ����� ����������� ������
        {
            if (!((parsedFormula[i + 1].isInBracket == 1) || (parsedFormula[i + 1].isUnOperator == 1) || (parsedFormula[i + 1].isVariable == 1)))
                return 0;
        }
        else if (parsedFormula[i].isVariable == 1)//�������� ����������� ���������� ��������� ����� ����������
        {
            if (!((parsedFormula[i + 1].isOutBracket == 1) || (parsedFormula[i + 1].isBinOperator == 1)))
                return 0;
        }
        else if (parsedFormula[i].isBinOperator == 1)//�������� ����������� ���������� ��������� ����� ��������� ���������
        {
            if (!((parsedFormula[i + 1].isInBracket == 1) || (parsedFormula[i + 1].isVariable == 1)))
                return 0;
        }
        else if (parsedFormula[i].isUnOperator == 1)//�������� ����������� ���������� ��������� ����� �������� ���������
        {
            if (!((parsedFormula[i + 1].isInBracket == 1) || (parsedFormula[i + 1].isVariable == 1 && parsedFormula[i + 2].isOutBracket == 1)))
                return 0;
        }
        else if (parsedFormula[i].isOutBracket == 1)//�������� ����������� ���������� ��������� ����� ����������� ������
        {
            if (!((parsedFormula[i + 1].isOutBracket == 1) || (parsedFormula[i + 1].isBinOperator == 1)))
                return 0;
        }
    }
    return 1;
}
bool VariablesCheck(string formula, set <string>& variables) //�������� ���������� �� ���������
{
    for (int i = 0; i < formula.size(); i++) //��������� ������ ������� �����
    { // �������� ����� � ��� ���������� ������� ��������������� ���������� �� "_"
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
    bool isThereNoWrongSymbols = 1; //��������� ������ �� ������� ������������ ��������
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
} //������

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

// ������� �������� �������������� �����
bool isFormula(string formula)
{
    if ((formula[0] == '1' || formula[0] == '0') && formula.size() == 1) //�������� ������� ��������� �� ����� ���������
        return 1;
    if ((formula[0] >= 65 && formula[0] <= 90) && formula.size() == 1) //�������� ������� ��������� �� ����� ���������
        return 1;
    /*if (formula[0] >= 65 && formula[0] <= 90)     //�������� ������� ��������� �� ����� ����������
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
    if (!Bra�ketCheck(formula))//�������� ������
        return 0;
    if (!AlphabetCheck(formula))//�������� ��������
        return 0;
    set <string> unicAtomicFormula;
    if (!VariablesCheck(formula, unicAtomicFormula))//�������� ����������� �� ���������
        return 0;
    if (!CheckOperators(formula))
        return 0;
    if (!LogicalCheck(formula)) // �������� �� ����������� ��������� � �������
        return 0;
    return 1;
}

void SearchForClosingBracket(string formula, int firstBracket, int& output) //����� ������� ����������� ������
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
} //������
void AddSubformula(string formula, int firstBracket, int secondBracket, set <string>& subFormuls) // �������� ���������� � ��������� ���������
{
    string substring = "";
    for (int i = firstBracket; i <= secondBracket; i++)
        substring.push_back(formula[i]);
    subFormuls.insert(substring);
} //������

set <string> FindSubformuls(string formula) // ����� ���������
{
    set <string> unicFormula;

    if ((formula[0] == '1' || formula[0] == '0') && formula.size() == 1) //�������� ������� ��������� �� ����� ���������
    {
        unicFormula.insert(formula);
        return unicFormula;
    }

    if (formula[0] >= 65 && formula[0] <= 90)     //���� ������� ���������� � �����
    {
        unicFormula.insert(formula);
        return unicFormula;
    }

    VariablesCheck(formula, unicFormula);// ����� ��������� ������


    int nextBracket = 0;
    for (int i = 0; i < formula.size(); i++) //����� ��������� � ������
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
        cout << "������� ������� ������ ������������: ";
        cin >> formula;
        if (isFormula(formula)) {
            cout << "������� ����������� �����" << endl;
            set <string> Subformuls;
            Subformuls = FindSubformuls(formula);
            cout << "���������� ���������� ��������� � �������� �������: ";
            cout << Subformuls.size() << endl;
            cout << "���������� ���������� �������� �������:" << endl;
            for (std::set<string>::iterator i = Subformuls.begin(); i != Subformuls.end(); ++i)
            {
                cout << *i << endl;
            }
        }
        else {
            cout << "������� �� ����������� �����" << endl;
        }
    }
    return 0;
}


