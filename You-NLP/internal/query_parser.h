#pragma once
#ifndef YOU_NLP_INTERNAL_QUERY_PARSER_H_
#define YOU_NLP_INTERNAL_QUERY_PARSER_H_

#include <boost/spirit/include/qi.hpp>

#include "parse_tree.h"
#include "parser.h"

namespace You {
namespace NLP {
namespace Internal {

/// The query parser that recognises our input syntax.
class QueryParser : public boost::spirit::qi::grammar<
	ParserIteratorType,
	QUERY(),
	ParserSkipperType> {
public:
	/// The type of the iterator used in this grammar.
	typedef ParserIteratorType IteratorType;

	/// The type of the skipper used in this grammar.
	typedef ParserSkipperType SkipperType;

	/// The type of the lexeme buffer provided in a lexing semantic action.
	typedef std::vector<ParserCharEncoding::char_type> LexemeType;

	/// The type of input strings accepted by this parser.
	typedef std::basic_string<ParserCharEncoding::char_type> StringType;

public:
	QueryParser();

	/// Parses the given input string into a query.
	///
	/// \param[in] string The string to parse.
	/// \return The query tree representing the given query.
	static QUERY parse(const StringType& string);

private:
	/// Process the nonterminal returned from the add query rule, converting it
	/// to an appropriate \ref ADD_QUERY type.
	///
	/// \param[in] lexeme The lexeme from the parser.
	/// \return The synthesised value for the \ref addCommand rule.
	static ADD_QUERY constructAddQuery(const LexemeType& lexeme);

	/// Process the nonterminal returned from the add query rule, when a
	/// deadline is specified, converting it to an appropriate \ref ADD_QUERY
	/// type.
	///
	/// \see constructAddQueryWithDeadlineTail
	/// This rule is two-part, because we need exhaustive backtracking.
	///
	/// \param[in] lexeme The lexeme from the parser. This is either the tail,
	///                   which contains the deadline and the rest of the
	///                   description; or the tail and one character of the
	///                   description.
	/// \return The synthesised value for the \ref addCommandWithDeadline rule.
	static ADD_QUERY constructAddQueryWithDeadline(const boost::variant<
		ADD_QUERY,
		boost::fusion::vector<ParserCharEncoding::char_type, ADD_QUERY>
	>& lexeme);

	/// Process the nonterminal returned from the add query rule, when a
	/// deadline is specified, converting it to an appropriate \ref ADD_QUERY
	/// type.
	///
	/// \see constructAddQueryWithDeadline
	/// This rule is two-part, because we need exhaustive backtracking.
	///
	/// \param[in] c The last character of the description.
	/// \param[in] deadline The deadline from the parser.
	/// \return The synthesised value for the \ref addCommandWithDeadline rule.
	static ADD_QUERY constructAddQueryWithDeadlineTail(
		const ParserCharEncoding::char_type c, const LexemeType& deadline);

	/// Handles failures in parsing. This raises a \ref ParseErrorException.
	///
	/// \exception ParseErrorException The exception representing the parse
	///                                error.
	static void onFailure(
		ParserIteratorType begin,
		ParserIteratorType end,
		ParserIteratorType errorPos,
		const boost::spirit::info& message);

private:
	/// The start rule.
	start_type start;

	/// Explicit command rule.
	boost::spirit::qi::rule<IteratorType, QUERY(), SkipperType> explicitCommand;

	/// Add command rule.
	boost::spirit::qi::rule<IteratorType, ADD_QUERY(), SkipperType> addCommand;

	/// Add command rule.
	boost::spirit::qi::rule<IteratorType, ADD_QUERY(), SkipperType>
		addCommandWithDescription;

	/// Add command's deadline rule.
	/// {
	boost::spirit::qi::rule<IteratorType, ADD_QUERY(), SkipperType>
		addCommandWithDeadline;
	boost::spirit::qi::rule<IteratorType, ADD_QUERY(), SkipperType>
		addCommandWithDeadlineTail;
	/// }
};

}  // namespace Internal
}  // namespace NLP
}  // namespace You

#endif  // YOU_NLP_INTERNAL_QUERY_PARSER_H_
