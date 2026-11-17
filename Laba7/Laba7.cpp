/**************************
 * Author: Ivanova Ksenia *
 * Variant: 6             *
 * Name: Laba 7           *
 **************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Contact {
  string lastName;
  string firstName;
  string birthDate;
  string address;
  string phone;
};

const int MAX_RECORDS = 100;

vector<string> splitFields(const string& line) {
  vector<string> fields;
  size_t start = 0;

  while (true) {
    size_t separator = line.find(';', start);
    if (separator == string::npos) {
      fields.push_back(line.substr(start));
      break;
    }
    fields.push_back(line.substr(start, separator - start));
    start = separator + 1;
  }

  return fields;
}

bool isValidDate(const string& date) {
  bool result = true;
  int year;
  int month;
  int day;
  int daysInMonth;

  if (date.size() != 10 || date[4] != '-' || date[7] != '-') {
    result = false;
  }

  if (result) {
    for (int i = 0; i < 10; ++i) {
      if (i != 4 && i != 7 && (date[i] < '0' || date[i] > '9')) {
        result = false;
      }
    }
  }

  if (result) {
    year = (date[0] - '0') * 1000 + (date[1] - '0') * 100
         + (date[2] - '0') * 10 + (date[3] - '0');
    month = (date[5] - '0') * 10 + (date[6] - '0');
    day = (date[8] - '0') * 10 + (date[9] - '0');

    if (month < 1 || month > 12 || day < 1) {
      result = false;
    } else {
      daysInMonth = 31;
      if (month == 4 || month == 6 || month == 9 || month == 11) {
        daysInMonth = 30;
      } else if (month == 2) {
        daysInMonth = 28;
        if (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)) {
          daysInMonth = 29;
        }
      }
      if (day > daysInMonth) {
        result = false;
      }
    }
  }

  return result;
}

bool readRecords(const string& inputPath, vector<Contact>& contacts) {
  ifstream input(inputPath);
  string line;
  int lineNumber = 0;

  if (!input) {
    cout << "ERROR input\n";
    return false;
  }

  while (getline(input, line)) {
    ++lineNumber;

    if (!line.empty() && line.back() == '\r') {
      line.pop_back();
    }

    vector<string> fields = splitFields(line);
    if (fields.size() != 5) {
      cout << "ERROR line " << lineNumber << "\n";
      return false;
    }
    if (fields[0].empty() || fields[1].empty() || fields[3].empty() || fields[4].empty()) {
      cout << "ERROR line " << lineNumber << "\n";
      return false;
    }
    if (!isValidDate(fields[2])) {
      cout << "ERROR line " << lineNumber << "\n";
      return false;
    }
    if ((int)contacts.size() >= MAX_RECORDS) {
      cout << "ERROR line " << lineNumber << "\n";
      return false;
    }

    Contact current;
    current.lastName = fields[0];
    current.firstName = fields[1];
    current.birthDate = fields[2];
    current.address = fields[3];
    current.phone = fields[4];
    contacts.push_back(current);
  }

  if (input.bad()) {
    cout << "ERROR read\n";
    return false;
  }

  return true;
}

string makeReport(const vector<Contact>& contacts, const string& street) {
  ostringstream report;
  int foundCount = 0;

  report << "Street: " << street << "\n";
  for (int i = 0; i < (int)contacts.size(); ++i) {
    if (contacts[i].address.find(street) != string::npos) {
      report << contacts[i].lastName << " " << contacts[i].firstName << " "
             << contacts[i].birthDate << " " << contacts[i].address << " "
             << contacts[i].phone << "\n";
      ++foundCount;
    }
  }

  if (foundCount == 0) {
    report << "EMPTY\n";
  }
  report << "Found: " << foundCount << "\n";

  if (contacts.empty()) {
    report << "Youngest: NONE\n";
    report << "Oldest: NONE\n";
  } else {
    int youngestIndex = 0;
    int oldestIndex = 0;
    for (int i = 1; i < (int)contacts.size(); ++i) {
      if (contacts[i].birthDate > contacts[youngestIndex].birthDate) {
        youngestIndex = i;
      }
      if (contacts[i].birthDate < contacts[oldestIndex].birthDate) {
        oldestIndex = i;
      }
    }
    report << "Youngest: " << contacts[youngestIndex].lastName << " "
           << contacts[youngestIndex].firstName << " "
           << contacts[youngestIndex].birthDate << "\n";
    report << "Oldest: " << contacts[oldestIndex].lastName << " "
           << contacts[oldestIndex].firstName << " "
           << contacts[oldestIndex].birthDate << "\n";
  }

  return report.str();
}

int main() {
  const string inputPath = "input.txt";
  const string outputPath = "output.txt";
  string street;
  vector<Contact> contacts;

  cout << "Street: ";
  getline(cin, street);

  if (!readRecords(inputPath, contacts)) {
    return 1;
  }

  string reportText = makeReport(contacts, street);

  ofstream output(outputPath);
  if (!output) {
    cout << "ERROR output\n";
    return 1;
  }

  output << reportText;
  output.flush();
  if (!output) {
    cout << "ERROR write\n";
    return 1;
  }

  cout << reportText;

  return 0;
}