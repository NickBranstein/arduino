#ifndef helpers_h
#define helpers_h

typedef void (*GenericFP)();

template<typename T, size_t S>
size_t length(T(&)[S]){
	return S;
}

bool isBell(int value)
{
  return value == 4 || value == 8 || value == 12 || value == 16 || value == 20 || value == 24;
}

#endif