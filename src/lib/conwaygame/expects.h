#pragma once


#define EXPECTS(condition) \
if (!(condition)) throw std::logic_error( \
std::string(__func__) + "() expects: "#condition)
