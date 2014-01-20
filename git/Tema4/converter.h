#ifndef CONVERTER_H
#define CONVERTER_H

#include <sstream>
#include <string>
#include <cstdarg>
#include <iostream>

class Converter
{
public:
	Converter();
	~Converter();

	/************************************************************************/
	/* Konwertuje zmienna do stringa                                        */
	/************************************************************************/
	template <class T>
	static std::string toString(T & _type)
	{
		std::string out;
		std::stringstream ss;

		ss<<_type;
		ss>>out;

		ss.clear();
		return out;
	}

	/************************************************************************/
	/* Konwertuje stringa do zmiennej, szablon potrzebuje informacji o typie*/
	/************************************************************************/
	template <class T>
	static T fromString(std::string & _string)
	{
		T out;
		std::stringstream ss;

		ss<<_string;
		ss>>out;

		ss.clear();
		return out;
	}

	/************************************************************************/
	/* Wypełnia tablicę podanymi danymi, length = ilosc danych				*/
	/************************************************************************/
	template <class T>
	static void setArray(T * array, int length, ...)
	{	
		va_list ap;
		va_start(ap, length);
		for (int i=0; i<length; i++)
		{
			array[i] = va_arg(ap,T);
		}
		va_end(ap);
	}

	/************************************************************************/
	/* Specjalizacja na float'a, z jakis powodow funkcja wariadyczna kastuje*/
	/* floaty na double														*/
	/************************************************************************/
	template <>
	static void setArray<float>(float * array, int length, ...)
	{
		va_list ap;
		va_start(ap, length);
		for (int i=0; i<length; i++)
		{
			array[i] = va_arg(ap,double);
		}
		va_end(ap);
	}
};

#endif