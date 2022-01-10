//
// Created by timur on 08.01.2022.
//

#pragma once
#include "../HeadersExtensions.h"
class IntegerParser {
  enum State{
	start,
	s1,
	s2,
	s3,
	error
  };
  bool IsDecimalNum(std::string Num);
  int ToNum(std::string Num);

public:
    bool InitializeNum(std::string Num,int &res);
};


