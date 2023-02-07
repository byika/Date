#include <iostream>
#include <cstddef>
#include <string>

class Date
{
public:
    Date();

    Date(
        const std::size_t year,
        const std::size_t month,
        const std::size_t day);

    Date(
        const std::size_t year,
        const std::size_t month,
        const std::size_t day,
        const std::string format);

    Date(
        const std::size_t year,
        const std::size_t month,
        const std::size_t day,
        const std::string format,
        const std::string delimiter);

    void set_year(const std::size_t year) { m_year = year; }
    void set_month(const std::size_t month) { m_month = month; }
    void set_day(const std::size_t day) { m_day = day; }
    void set_format(const std::string format) { m_format = format; }
    void set_delimiter(char delimiter) { m_delimiter = delimiter; }

    std::size_t get_year() const { return m_year; }
    std::size_t get_month() const { return m_month; }
    std::size_t get_day() const { return m_day; }
    std::string get_format() const { return m_format; }
    std::string get_delimiter() const { return m_delimiter; }

    void increase();
    void reset_to_defaults();
    bool is_leap_year() const;

    friend std::ostream& operator<<(std::ostream& os, const Date& d);
    friend std::istream& operator>>(std::istream& is, Date& d);

private:
    std::size_t m_year;
    std::size_t m_month;
    std::size_t m_day;
    std::string m_format;
    std::string m_delimiter;

    const std::size_t DEFAULT_YEAR = 1970;
    const std::size_t DEFAULT_MONTH = 1;
    const std::size_t DEFAULT_DAY = 1;
    const std::string DEFAULT_FORMAT = "yyyymmdd";
    const std::string DEFAULT_DELIMITER = ".";
    enum { JAN = 1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC };

    bool is_february() const;
    bool is_28day_february() const;
    bool is_29day_february() const;
    bool is_30day_month() const;
    bool is_31day_month() const;
    bool is_valid_date() const;

    bool is_last_day_of_the_year() const;
    bool is_last_day_of_the_month() const;
};

Date::Date()
{
    reset_to_defaults();
}

Date::Date(
    const std::size_t year,
    const std::size_t month,
    const std::size_t day)
{
    m_year = year;
    m_month = month;
    m_day = day;
    m_format = DEFAULT_FORMAT;
    m_delimiter = DEFAULT_DELIMITER;

    if (!is_valid_date())
    {
        reset_to_defaults();
    }
}

Date::Date(
    const std::size_t year,
    const std::size_t month,
    const std::size_t day,
    const std::string format)
{
    m_year = year;
    m_month = month;
    m_day = day;
    m_format = format;
    m_delimiter = DEFAULT_DELIMITER;

    if (!is_valid_date())
    {
        reset_to_defaults();
    }
}

Date::Date(
    const std::size_t year,
    const std::size_t month,
    const std::size_t day,
    const std::string format,
    const std::string delimiter)
{
    m_year = year;
    m_month = month;
    m_day = day;
    m_format = format;
    m_delimiter = delimiter;

    if (!is_valid_date())
    {
        reset_to_defaults();
    }
}

void Date::increase()
{
    if (is_last_day_of_the_year())
    {
        ++m_year;
        m_month = 1;
        m_day = 1;
    }
    else if (is_last_day_of_the_month())
    {
        ++m_month;
        m_day = 1;
    }
    else
    {
        ++m_day;
    }
}

void Date::reset_to_defaults()
{
    m_year = DEFAULT_YEAR;
    m_month = DEFAULT_MONTH;
    m_day = DEFAULT_DAY;
    m_format = DEFAULT_FORMAT;
    m_delimiter = DEFAULT_DELIMITER;
}

std::ostream& operator<<(std::ostream& os, const Date& d)
{
    if (d.m_format == "ddmmyyyy")
    {
        os << d.m_day << d.m_delimiter << d.m_month << d.m_delimiter << d.m_year;
    }
    else if (d.m_format == "mmddyyyy")
    {
        os << d.m_month << d.m_delimiter << d.m_day << d.m_delimiter << d.m_year;
    }
    else if (d.m_format == "yyyyddmm")
    {
        os << d.m_year << d.m_delimiter << d.m_day << d.m_delimiter << d.m_month;
    }
    else
    {
        os << d.m_year << d.m_delimiter << d.m_month << d.m_delimiter << d.m_day;
    }

    return os;
}

std::istream& operator>>(std::istream& is, Date& d)
{
    is >> d.m_year >> d.m_month >> d.m_day;
    if (!d.is_valid_date())
    {
        d.reset_to_defaults();
    }
    return is;
}

bool Date::is_leap_year() const
{
    return ((m_year % 4 == 0) && (m_year % 100 != 0)) || (m_year % 400 == 0);
}

bool Date::is_february() const
{
    return m_month == FEB;
}

bool Date::is_28day_february() const
{
    return m_month == FEB && !is_leap_year();
}

bool Date::is_29day_february() const
{
    return m_month == FEB && is_leap_year();
}

bool Date::is_30day_month() const
{
    return m_month == APR || m_month == JUN || m_month == SEP || m_month == NOV;
}

bool Date::is_31day_month() const
{
    return !is_february() && !is_30day_month();
}

bool Date::is_valid_date() const
{
    bool result = true;

    if (m_day > 31)
    {
        result = false;
    }

    if (m_month > 12)
    {
        result = false;
    }

    if (is_28day_february() && m_day > 28)
    {
        result = false;
    }

    if (is_29day_february() && m_day > 29)
    {
        result = false;
    }

    if (is_30day_month() && m_day > 30)
    {
        result = false;
    }

    return result;
}

bool Date::is_last_day_of_the_year() const
{
    return m_month == DEC && m_day == 31;
}

bool Date::is_last_day_of_the_month() const
{
    bool result = true;

    if (
        (is_28day_february() && m_day != 28) ||
        (is_29day_february() && m_day != 29) ||
        (is_30day_month() && m_day != 30) ||
        (is_31day_month() && m_day != 31)
        )
    {
        result = false;
    }

    return result;
}

int main()
{
    /*
    Date defaultDate;
    std::cout << defaultDate << "\n";

    const std::size_t FIVE_WEEKS = 35;

    for (std::size_t i = 0; i < FIVE_WEEKS; ++i)
    {
        defaultDate.increase();
        std::cout << defaultDate << "\n";
    }

    Date today(2023, 2, 7);
    std::cout << "\n\n" << today << "\n";

    for (std::size_t i = 0; i < FIVE_WEEKS; ++i)
    {
        today.increase();
        std::cout << today << "\n";
    }

    Date leapYearDate(2023, 6, 23, "ddmmyyyy", "-");
    std::cout << std::boolalpha << leapYearDate.is_leap_year() << std::endl;

    Date notLeapYearDate;
    for (std::size_t year = 1700; year <= 1900; year += 100)
    {
        notLeapYearDate.set_year(year);
        std::cout << year << std::boolalpha << notLeapYearDate.is_leap_year() << std::endl;
    }
    */
    std::cout << "BBeDuTe DaTY Bashego PojDeHuya (yyyymmdd)" << std::endl;
    Date yourBirthdayDate;
    std::cin >> yourBirthdayDate;
    std::cout << "You was born in " << yourBirthdayDate << "\n\n";
}