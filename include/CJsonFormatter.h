#ifndef _CJSONFORMATTER_H_
#define _CJSONFORMATTER_H_

#include <streambuf>
#include <ostream>

/**
 * @brief CJsonFormatter
 *        Wraps a std::ostream and automatically edits styles/indentation
 */
class CJsonFormatter : protected std::streambuf, public std::ostream {
public:
    using int_type = std::streambuf::int_type;

    /**
     * @brief How many spaces are currently being appended at every new line
     */
    unsigned int spaces = 0;

    /**
     * @brief How much 'unsigned int spaces' should be incremented/decremented by each call to Indent() or Unindent()
     */
    unsigned int indent = 4;

    /**
     * @brief Creates this wrapper around another std::ostream
     * @param _out The actual output to receive the processed data
     */
    explicit CJsonFormatter( std::ostream& _out );

    /**
     * @brief Increments the number of spaces appended to each new line by the value of 'indent'
     */
    void Indent();

    /**
     * @brief Decrements the number of spaces appended to each new line by the value of 'indent'
     */
    void Unindent();

private:
    CJsonFormatter(CJsonFormatter&&) = delete;
    CJsonFormatter(const CJsonFormatter&) = delete;
    CJsonFormatter& operator=(const CJsonFormatter&) = delete;

protected:
    std::ostream& out;
    virtual int_type overflow (int_type c) override;
};

/**
 * @brief CJsonIndenterControl
 *        Represents a command that can be pushed into an std::ostream to configure the underlying CJsonFormatter
 */
class CJsonFormatterControl;
std::ostream& operator<<(std::ostream& os, const CJsonFormatterControl& c);
CJsonFormatter& operator<<(CJsonFormatter& os, const CJsonFormatterControl& c);

/**
 * @brief Causes Indent() to be invoked on the underlying CJsonFormatter stream, or no effect on a regular std::ostream
 */
extern const CJsonFormatterControl& json_indent;

/**
 * @brief Causes Unindent() to be invoked on the underlying CJsonFormatter stream, or no effect on a regular std::ostream
 */
extern const CJsonFormatterControl& json_unindent;

/**
 * @brief Causes a new line ('\n') character to be printed if using CJsonFormatter, or a space (' ') on a regular std::ostream
 */
extern const CJsonFormatterControl& json_endl;

#endif // _CJSONINDENTER_H_
