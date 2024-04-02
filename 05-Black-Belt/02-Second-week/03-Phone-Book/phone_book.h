#pragma once

#include "iterator_range.h"

#include <optional>
#include <string>
#include <string_view>
#include <vector>
#include <iosfwd>
#include <map>
#include <algorithm>
#include <utility>

struct Date {
    int year, month, day;
};

struct Contact {
    std::string name;
    std::optional<Date> birthday;
    std::vector<std::string> phones;
};

class PhoneBook {
public:
    using SortedContacts = std::vector<Contact>;
    using SortedContactsIt = SortedContacts::const_iterator;
    using ContactRange = IteratorRange<SortedContactsIt>;
    
    explicit PhoneBook(std::vector<Contact> contacts) {
        if (contacts.size()) {
            std::sort(contacts.begin(), contacts.end(), PhoneBook::less);
            data = std::move(contacts);
        }
    }

    ContactRange FindByNamePrefix(std::string_view name_prefix) const;

    void SaveTo(std::ostream& output) const;
private:
    std::vector<Contact> data;
    static bool less(const Contact& lhs, const Contact& rhs) {
        return lhs.name < rhs.name;
    };
};

PhoneBook DeserializePhoneBook(std::istream& input);

