#include <bits/stdc++.h>

using namespace std;

struct Attribute {
    string name;
    string value;
};

struct Table {
    string name;
    vector<string> attributeNames;
    vector<vector<Attribute>> records;
};

Table CreateTable(const string& name, const vector<string>& attributeNames) {
    Table table;
    table.name = name;
    table.attributeNames = attributeNames;
    return table;
}

void InsertRecord(Table& table, const vector<Attribute>& record) {
    if (record.size() == table.attributeNames.size()) {
        table.records.push_back(record);
    } else {
        cout << "Record size does not match the number of attributes." << endl;
    }
}

void DisplayTable(const Table& table) {
    cout << "Table: " << table.name << endl;

    for (const string& attributeName : table.attributeNames) {
        cout << attributeName << "\t";
    }
    cout << endl;

    for (const vector<Attribute>& record : table.records) {
        for (const Attribute& attribute : record) {
            cout << attribute.value << "\t";
        }
        cout << endl;
    }
}

Table Selection(const Table& table, const string& condition) {
    Table result = table;
    result.name = "Selection";

    vector<Attribute> attributes;
    vector<string> operators = {"<", "<=", "=", ">", ">=", "!="};

    string attributeName, op, value;
    bool attributeFound = false;

    for (const string& oper : operators) {
        size_t pos = condition.find(oper);
        if (pos != string::npos) {
            attributeName = condition.substr(0, pos);
            op = oper;
            value = condition.substr(pos + oper.size());
            attributeFound = true;
            break;
        }
    }

    if (!attributeFound) {
        cout << "Invalid condition. Must use one of the operators: <, <=, =, >, >=, !=" << endl;
        return Table();
    }

    int attributeIndex = -1;
    for (size_t i = 0; i < result.attributeNames.size(); i++) {
        if (result.attributeNames[i] == attributeName) {
            attributeIndex = i;
            break;
        }
    }

    if (attributeIndex == -1) {
        cout << "Attribute '" << attributeName << "' not found in the table." << endl;
        return Table();
    }

    for (size_t i = 0; i < result.records.size(); ) {
        if (op == "<") {
            if (result.records[i][attributeIndex].value < value)
                i++;
            else
                result.records.erase(result.records.begin() + i);
        }
        else if (op == "<=") {
            if (result.records[i][attributeIndex].value <= value)
                i++;
            else
                result.records.erase(result.records.begin() + i);
        }
        else if (op == "=") {
            if (result.records[i][attributeIndex].value == value)
                i++;
            else
                result.records.erase(result.records.begin() + i);
        }
        else if (op == ">") {
            if (result.records[i][attributeIndex].value > value)
                i++;
            else
                result.records.erase(result.records.begin() + i);
        }
        else if (op == ">=") {
            if (result.records[i][attributeIndex].value >= value)
                i++;
            else
                result.records.erase(result.records.begin() + i);
        }
        else if (op == "!=") {
            if (result.records[i][attributeIndex].value != value)
                i++;
            else
                result.records.erase(result.records.begin() + i);
        }
    }

    return result;
}

Table Rename(const Table& table, const string& oldName, const string& newName) {
    Table result = table;
    result.name = "Rename";

    int attributeIndex = -1;
    for (size_t i = 0; i < result.attributeNames.size(); i++) {
        if (result.attributeNames[i] == oldName) {
            attributeIndex = i;
            break;
        }
    }

    if (attributeIndex == -1) {
        cout << "Attribute '" << oldName << "' not found in the table." << endl;
        return Table();
    }

    result.attributeNames[attributeIndex] = newName;

    return result;
}

Table NaturalJoin(const Table& table1, const Table& table2) {
    Table result;
    result.name = "NaturalJoin";

    for (const string& attr1 : table1.attributeNames) {
        result.attributeNames.push_back(attr1);
    }
    for (const string& attr2 : table2.attributeNames) {
        bool existsInTable1 = false;
        for (const string& attr1 : table1.attributeNames) {
            if (attr2 == attr1) {
                existsInTable1 = true;
                break;
            }
        }
        if (!existsInTable1) {
            result.attributeNames.push_back(attr2);
        }
    }

    for (const vector<Attribute>& record1 : table1.records) {
        for (const vector<Attribute>& record2 : table2.records) {
            bool isDuplicate = false;
            for (const Attribute& attr1 : record1) {
                for (const Attribute& attr2 : record2) {
                    if (attr1.name == attr2.name) {
                        if (attr1.value != attr2.value) {
                            isDuplicate = true;
                            break;
                        }
                    }
                }
                if (isDuplicate) {
                    break;
                }
            }

            if (!isDuplicate) {
                vector<Attribute> combinedRecord = record1;
                for (const Attribute& attr2 : record2) {
                    bool existsInTable1 = false;
                    for (const Attribute& attr1 : record1) {
                        if (attr2.name == attr1.name) {
                            existsInTable1 = true;
                            break;
                        }
                    }
                    if (!existsInTable1) {
                        combinedRecord.push_back(attr2);
                    }
                }
                result.records.push_back(combinedRecord);
            }
        }
    }

    return result;
}

Table Union(const Table& table1, const Table& table2) {
    if (table1.attributeNames != table2.attributeNames) {
        cout << "Union operation requires tables with the same attributes." << endl;
        return Table();
    }

    Table result = table1;
    result.name = "Union";

    for (const vector<Attribute>& record : table2.records) {
        bool isDuplicate = false;
        for (const vector<Attribute>& existingRecord : result.records) {
            if (existingRecord.size() == record.size()) {
                bool isEqual = true;
                for (size_t i = 0; i < record.size(); i++) {
                    if (existingRecord[i].value != record[i].value) {
                        isEqual = false;
                        break;
                    }
                }
                if (isEqual) {
                    isDuplicate = true;
                    break;
                }
            }
        }

        if (!isDuplicate) {
            result.records.push_back(record);
        }
    }

    return result;
}

Table Projection(const Table& table, const vector<string>& attributeNames) {
    Table result;
    result.name = "Projection";

    for (const string& attributeName : attributeNames) {
        bool existsInTable = false;
        for (const string& attr : table.attributeNames) {
            if (attributeName == attr) {
                existsInTable = true;
                break;
            }
        }
        if (!existsInTable) {
            cout << "Attribute '" << attributeName << "' not found in the table." << endl;
            return Table();
        }
    }

    result.attributeNames = attributeNames;

    for (const vector<Attribute>& record : table.records) {
        vector<Attribute> projectedRecord;
        for (const string& attributeName : attributeNames) {
            for (const Attribute& attribute : record) {
                if (attribute.name == attributeName) {
                    projectedRecord.push_back(attribute);
                    break;
                }
            }
        }
        result.records.push_back(projectedRecord);
    }

    return result;
}

Table Difference(const Table& table1, const Table& table2) {
    if (table1.attributeNames != table2.attributeNames) {
        cout << "Difference operation requires tables with the same attributes." << endl;
        return Table();
    }

    Table result = table1;
    result.name = "Difference";

    for (const vector<Attribute>& record : table2.records) {
        for (auto it = result.records.begin(); it != result.records.end(); ) {
            if (it->size() == record.size()) {
                bool isEqual = true;
                for (size_t i = 0; i < record.size(); i++) {
                    if ((*it)[i].value != record[i].value) {
                        isEqual = false;
                        break;
                    }
                }
                if (isEqual) {
                    it = result.records.erase(it);
                } else {
                    ++it;
                }
            } else {
                ++it;
            }
        }
    }

    return result;
}

Table Intersection(const Table& table1, const Table& table2) {
    if (table1.attributeNames != table2.attributeNames) {
        cout << "Intersection operation requires tables with the same attributes." << endl;
        return Table();
    }

    Table result = table1;
    Table result2;
    result2.name = "Intersection";

    for (const vector<Attribute>& record : table2.records) {
        bool isDuplicate = false;
        for (const vector<Attribute>& existingRecord : result.records) {
            if (existingRecord.size() == record.size()) {
                bool isEqual = true;
                for (size_t i = 0; i < record.size(); i++) {
                    if (existingRecord[i].value != record[i].value) {
                        isEqual = false;
                        break;
                    }
                }
                if (isEqual) {
                    isDuplicate = true;
                    break;
                }
            }
        }

        if (isDuplicate) {
            result.records.push_back(record);
            result2.records.push_back(record);
        }
    }

    return result2;
}

Table Division(const Table& table1, const Table& table2) {
    Table result;
    result.name = "Division";

    for (const string& attr1 : table1.attributeNames) {
        bool existsInTable2 = false;
        for (const string& attr2 : table2.attributeNames) {
            if (attr1 == attr2) {
                existsInTable2 = true;
                break;
            }
        }
        if (!existsInTable2) {
            cout << "Attribute '" << attr1 << "' not found in the table 2." << endl;
            return Table();
        }
    }

    for (const string& attr2 : table2.attributeNames) {
        bool existsInTable1 = false;
        for (const string& attr1 : table1.attributeNames) {
            if (attr2 == attr1) {
                existsInTable1 = true;
                break;
            }
        }
        if (!existsInTable1) {
            cout << "Attribute '" << attr2 << "' not found in the table 1." << endl;
            return Table();
        }
    }

    if (table2.records.empty()) {
        cout << "Division by empty table." << endl;
        return Table();
    }

    result.attributeNames = table1.attributeNames;

    for (const vector<Attribute>& record1 : table1.records) {
        bool isRecordDivisible = false;
        for (const vector<Attribute>& record2 : table2.records) {
            bool isEqual = true;
            for (size_t i = 0; i < record1.size(); i++) {
                if (record1[i].value != record2[i].value) {
                    isEqual = false;
                    break;
                }
            }
            if (isEqual) {
                isRecordDivisible = true;
                break;
            }
        }
        if (isRecordDivisible) {
            result.records.push_back(record1);
        }
    }

    return result;
}


Table CartesianProduct(const Table& table1, const Table& table2) {
    Table result;
    result.name = "CartesianProduct";

    result.attributeNames = table1.attributeNames;
    result.attributeNames.insert(result.attributeNames.end(), table2.attributeNames.begin(), table2.attributeNames.end());

    for (const vector<Attribute>& record1 : table1.records) {
        for (const vector<Attribute>& record2 : table2.records) {
            vector<Attribute> combinedRecord = record1;
            combinedRecord.insert(combinedRecord.end(), record2.begin(), record2.end());
            result.records.push_back(combinedRecord);
        }
    }

    return result;
}

int main() {
    Table table1 = CreateTable("Students", {"Name", "Age", "GPA","temp"});
    InsertRecord(table1, {{"Name", "Alice"}, {"Age", "20"}, {"GPA", "3.5"},{"temp","2"}});
    InsertRecord(table1, {{"Name", "Bob"}, {"Age", "22"}, {"GPA", "3.2"},{"temp","2"}});
    InsertRecord(table1, {{"Name", "Jaya"}, {"Age", "20"}, {"GPA", "4"},{"temp","2"}});

    Table table2 = CreateTable("Courses", {"Name", "Credits","temp"});
    InsertRecord(table2, {{"Name", "Math"}, {"Credits", "3"},{"temp","2"}});
    InsertRecord(table2, {{"Name", "Science"}, {"Credits", "4"},{"temp","2"}});
    InsertRecord(table2, {{"Name", "Telugu"}, {"Credits", "4"},{"temp","2"}});
    InsertRecord(table2, {{"Name", "Jaya"}, {"Credits", "4"},{"temp","2"}});

    Table table3 = CreateTable("Students", {"Name", "Age", "GPA"});
    InsertRecord(table3, {{"Name", "Alice"}, {"Age", "20"}, {"GPA", "3.5"}});  
    InsertRecord(table3, {{"Name", "Bob"}, {"Age", "22"}, {"GPA", "3.2"}});

    Table table4 = CreateTable("students1", {"Name", "Age", "GPA","temp"});
    InsertRecord(table4, {{"Name", "Alice"}, {"Age", "20"}, {"GPA", "3.5"},{"temp","2"}});
    InsertRecord(table4, {{"Name", "Bob"}, {"Age", "22"}, {"GPA", "3.2"},{"temp","2"}});

    int m;
    cout<<"choose the operation to be performed"<<endl;
    cout<<"1.Display table 1"<<endl;
    cout<<"2.Display table 2"<<endl;
    cout<<"3.Selection usage "<<endl;
    cout<<"4.Rename the attribute Age to StudentAge in the table1"<<endl;
    cout<<"5.Natural join of table1 and table2"<<endl;
    cout<<"6.Union of table1 and table2"<<endl;
    cout<<"7.Intersection of table1 and table2"<<endl;
    cout<<"8.Difference of table1 and table2"<<endl;
    cout<<"9.Projection of table1 on Name and GPA"<<endl;
    cout<<"10.Division of table1 and table3"<<endl;
    cout<<"11.Cartesian product of table1 and table2"<<endl;
	cout<<"12.For any extra usages or implementations"<<endl;


    while(cin>>m){
        if(m==1){
            DisplayTable(table1);
        }
        if(m==2){
            DisplayTable(table2);
        }
        if(m==3){
            string s;
    while (true) {
        cout << "Enter condition: ";
        cin >> s;
        Table selectionResult = Selection(table1, s);
        DisplayTable(selectionResult);

        string op;
        cout << "Enter operation (AND, OR, or DONE to exit): ";
        cin >> op;

        if (op == "AND") {
            string s2;
            cout << "Enter second condition: ";
            cin >> s2;
            Table selectionResult2 = Selection(table1, s2);
            Table combinedResult = NaturalJoin(selectionResult, selectionResult2);
            DisplayTable(combinedResult);
        } else if (op == "OR") {
            string s2;
            cout << "Enter second condition: ";
            cin >> s2;
            Table selectionResult2 = Selection(table1, s2);
            Table combinedResult = Union(selectionResult, selectionResult2);
            DisplayTable(combinedResult);
        } else if (op == "DONE") {
            break;
        } else {
            cout << "Invalid operation. Please enter AND, OR, or DONE to exit." << endl;
			break;
        }
        }
		}
        if(m==4){
            Table renameResult = Rename(table1, "Age", "StudentAge");
            DisplayTable(renameResult);
        }
        if(m==5){
            Table joinResult = NaturalJoin(table1, table3);
            DisplayTable(joinResult);
        }
        if(m==6){
            Table unionResult = Union(table1, table2);
            DisplayTable(unionResult);
        }
        if(m==7){
            Table intersectionResult = Intersection(table1, table4);
            DisplayTable(intersectionResult);
        }
        if(m==8){
            Table differenceResult = Difference(table1, table4);
            DisplayTable(differenceResult);
        }
        if(m==9){
            Table projectionResult = Projection(table1, {"Name", "GPA"});
            DisplayTable(projectionResult);
        }
        if(m==10){
            Table divisionResult = Division(table1, table3);
            DisplayTable(divisionResult);
        }
        if(m==11){
            Table cartesianProductResult = CartesianProduct(table1, table2);
            DisplayTable(cartesianProductResult);
        }
		if(m==12){

		}
    }
    return 0;
}
