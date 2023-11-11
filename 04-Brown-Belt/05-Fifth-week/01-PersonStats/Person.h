//
//  Person.h
//  BrownBelt
//
//  Created by Ivan Tsylin on 12.08.2023.
//

#ifndef Person_h
#define Person_h

enum class Gender {
  FEMALE,
  MALE
};

struct Person {
  int age;  // возраст
  Gender gender;  // пол
  bool is_employed;  // имеет ли работу
};


#endif /* Person_h */
