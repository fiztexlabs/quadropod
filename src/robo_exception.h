#pragma once

#include <stdexcept>
#include <string>

namespace robo
{
	/**
	* @brief Basic RSP eception class
	*/
	class Exception :
		public std::runtime_error
	{
	public:
		Exception(const std::string& m)
			: std::runtime_error(m),
			type_{ "Basic" }
		{

		}
		virtual const std::string& type() const
		{
			return type_;
		};

	protected:
		std::string type_;
	};

	/**
	* @brief Exception invalid values
	*/
	class ExceptionInvalidValue :
		public Exception
	{
	public:
		ExceptionInvalidValue(const std::string& m)
			: Exception(m)
		{
			type_ = "InvalidValue";
		}
	};
}