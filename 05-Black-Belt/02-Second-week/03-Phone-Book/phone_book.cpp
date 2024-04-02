#include "phone_book.h"
#include "contact.pb.h"

#include <iterator>
#include <utility>
#include <algorithm>

using namespace std;

class ContactNamePrefixComp {
public:
    bool operator()(const Contact& lhs, std::string_view rhs_name) {
        string_view lhs_name(lhs.name);
        auto min_size = rhs_name.size();
        return lhs_name.substr(0, min_size) < rhs_name.substr(0, min_size);
    }
    bool operator()(std::string_view lhs_name, const Contact& rhs) {
        std::string_view rhs_name{rhs.name};
        auto min_size = lhs_name.size();
        return lhs_name.substr(0, min_size) < rhs_name.substr(0, min_size);
    }
};

PhoneBook::ContactRange PhoneBook::FindByNamePrefix(std::string_view name_prefix) const {
    if (name_prefix == "") {
        return ContactRange{data.begin(), data.end()};
    }

    auto range = equal_range(
                             data.begin(),
                             data.end(),
                             name_prefix,
                             ContactNamePrefixComp{}
                             );
    return ContactRange{range.first, range.second};
}

void PhoneBook::SaveTo(std::ostream& output) const {
    PhoneBookSerialize::ContactList target;
    for (const Contact& contact : data) {
        PhoneBookSerialize::Contact target_contract;
        target_contract.set_name(contact.name);
        if (contact.birthday.has_value()) {
            PhoneBookSerialize::Date target_date;
            target_date.set_day(contact.birthday.value().day);
            target_date.set_month(contact.birthday.value().month);
            target_date.set_year(contact.birthday.value().year);
            *target_contract.mutable_birthday() = target_date;
        }
        for (const string& phone : contact.phones) {
            target_contract.add_phone_number(phone);
        }
        *target.add_contact() = std::move(target_contract);
    }
    target.SerializePartialToOstream(&output);
}

PhoneBook DeserializePhoneBook(std::istream& input) {
    PhoneBookSerialize::ContactList source;
    vector<Contact> result;
    if (source.ParsePartialFromIstream(&input)) {
        for (const PhoneBookSerialize::Contact& source_contact : source.contact()) {
            Contact contact;
            contact.name = source_contact.name();
            if (source_contact.has_birthday()) {
                contact.birthday = {source_contact.birthday().year(), source_contact.birthday().month(), source_contact.birthday().day()};
            }
            for (const string& source_phone_number : source_contact.phone_number()) {
                contact.phones.emplace_back(source_phone_number);
            }
            result.emplace_back(move(contact));
        }
    }
    return PhoneBook(result);
}
