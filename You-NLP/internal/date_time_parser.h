#pragma once
#ifndef YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_
#define YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_

#include <boost/date_time/gregorian/gregorian_types.hpp>
#include "parser.h"

namespace You {
namespace NLP {
namespace Internal {
namespace UnitTests { class DateTimeParserTests; }

/// The query parser that recognises our input syntax.
class DateTimeParser : public boost::spirit::qi::grammar<
	ParserIteratorType,
	boost::posix_time::ptime(),
	ParserSkipperType> {
public:
	friend class UnitTests::DateTimeParserTests;

	/// The type of the iterator used in this grammar.
	typedef ParserIteratorType IteratorType;

	/// The type of the skipper used in this grammar.
	typedef ParserSkipperType SkipperType;

	/// The type of the lexeme buffer provided in a lexing semantic action.
	typedef std::vector<ParserCharEncoding::char_type> LexemeType;

	/// The type of input strings accepted by this parser.
	typedef std::basic_string<ParserCharEncoding::char_type> StringType;

public:
	/// Parses the given input string into date.
	///
	/// \param[in] string The string to parse.
	/// \return The date/time representing the string.
	static boost::posix_time::ptime parse(const StringType& string);

private:
	DateTimeParser();

	/// Parses a partial date into its boost::date_time form.
	///
	/// \param[in] dateParts the parts from the various sub-parsers.
	static boost::gregorian::date parsePartialDate(
		const boost::variant<
			int16_t,
			boost::fusion::vector<int16_t, boost::gregorian::months_of_year>
		> & dateParts);

	/// Parses the given year string into a full year. If this has only 2 digits
	/// then it will be treated as fuzzy; otherwise it will be a full year. This
	/// conversion will happen unless the year string is prefixed by a zero.
	///
	/// \param[in] chars The character string, containing ASCII 0-9.
	static int16_t parseFuzzyYear(
		const std::vector<ParserCharEncoding::char_type>& chars);

	/// Parses the given two-digit year into a full year according to system
	/// settings (on Windows) or using strptime conversion rules.
	///
	/// \param[in] year The year to convert. This must be between 0 and 99
	///                 inclusive.
	/// \return The actual year referenced.
	static int16_t parseTwoDigitYear(int16_t year);

private:
	/// The start rule.
	start_type start;

	/// Parsing dates.
	boost::spirit::qi::rule<
		IteratorType,
		boost::gregorian::date(),
		SkipperType> date;

	/// Parsing years.
	boost::spirit::qi::rule<IteratorType, int16_t(), SkipperType> year;

	/// Parsing months.
	boost::spirit::qi::rule<
		IteratorType,
		boost::gregorian::months_of_year(),
		SkipperType> month;
};

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_INTERNAL_DATE_TIME_PARSER_H_
