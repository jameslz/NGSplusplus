#include "uToken.h"
#include "utility/utility.h"
//enum class token_param { CHR, START_POS, END_POS, STRAND, MAP_SCORE, PHRED_SCORE, CIGAR, SEQUENCE, SEQ_NAME, FLAGS };

/** \brief uToken constructor.
 * \param istream& paramList: a stream containing all the parameters in the format: <token_param>\t<value>\n<token_param>\t<value>\n...
 */
uToken::uToken(std::istream& paramList)
{
    char line[1024];
    while (paramList.getline(line, 1024))
    {
        std::stringstream ss;
        ss << line;
        /**< Fetch name */
        token_param name;
        try
        {
            ss >> name;
        }
        catch (invalid_token_param_throw& e)
        {
            std::string trace;
            if (std::string const * ste =boost::get_error_info<invalid_token_param_error>(e))
                trace = *ste;
            invalid_uToken_throw err;
            err << invalid_uToken_error(trace);
            throw err;
        }
        /**< Fetch value */
        std::string value;
        ss >> value;
        try
        {
            _setParam(name, value);
        }
        catch (invalid_value_throw& e)
        {
            std::string trace;
            if (std::string const * ste =boost::get_error_info<invalid_value_error>(e))
                trace = *ste;
            invalid_uToken_throw err;
            err << invalid_uToken_error(trace);
            throw err;
        }
    }
    /**< Check if uToken is in a valid state  */
    try
    {
        _validateToken();
    }
    catch (invalid_uToken_error& e)
    {
        throw e;
    }
}

uToken& uToken::operator=(uToken const& assign_from)
{
    if (this == &assign_from) return *this;
    m_params = assign_from.m_params;
    return *this;
}

std::string uToken::getParam(token_param name) const
{
    if(isParamSet(name))
        return (m_params.find(name)->second);
    else
    {
        param_not_found e;
        e<<string_error("Tried to getParam that is not set, see token_param_type_error for type\n");
        e<<token_param_type_error(name);
        throw e;
    }
}



//std::string uToken::getParam(token_param name) {
//	return m_params[name];
//}

/** \brief Set a param only if it's format is valid
 * \param token_param& name: type of param
 * \param const std::string& value: the value of the parameter
 */
void uToken::_setParam(const token_param& name, const std::string& value)
{
    // TODO: What should we do in case the name is already setted? Overwrite silently, warning or error?
    try
    {
        if (_validateParam(name, value) == false)
        {
            invalid_value_throw e;
            e << invalid_value_error(value);
            throw e;
        }

        _postProcessParam(name, value);
        m_params[name] = value;
    }
    catch(invalid_value_throw &e)
    {
        throw e;
    }
//	m_params.insert(make_pair(name, value));
}

bool uToken::isParamSet(const token_param& name)const
{
    return m_params.count(name);
}

/** \brief Operate any necessary post_processing on our values. Note this may include setting values to the token
 *
 * \param name token_param& Type of parameter
 * \param value const std::string& Value of parameter
 * \return void
 *
 */
void uToken::_postProcessParam(const token_param& name, const std::string& value)
{

    switch(name)
    {
    case token_param::START_POS:
    case token_param::END_POS:
        break;
    case token_param::STRAND:
        break;
    case token_param::MAP_SCORE:
        break;
    case token_param::PHRED_SCORE:
        break;
    case token_param::CIGAR:
        _postProcCigar(value);
        break;
    case token_param::SEQUENCE:
        break;
    case token_param::FLAGS:
        _postProcFlag(value);
        break;
    case token_param::CHR:
        break;
    case token_param::SEQ_NAME:
        break;
    default:
        break;
    }
}

/** \brief Validate the parameter. Some tests are general, other are specific for each parameter.
 * \param token_param& name: type of param
 * \param const std::string& value: the value of the parameter
 */
bool uToken::_validateParam(const token_param& name, const std::string& value) const
{
    bool isValid = false;;
    /**< In every case, an empty string is an invalid value */
    if (value.size() == 0)
    {
        return false;
    }
    /**< Then we do basic check for every type of parameters */
    switch(name)
    {
    case token_param::START_POS:
    case token_param::END_POS:
        isValid = _posIsValid(value);
        break;
    case token_param::STRAND:
        isValid = _strandIsValid(value);
        break;
    case token_param::MAP_SCORE:
        isValid = _mapScoreIsValid(value);
        break;
    case token_param::PHRED_SCORE:
        isValid = true;
        break;
    case token_param::CIGAR:
        isValid = _cigarIsValid(value);
        break;
    case token_param::SEQUENCE:
        isValid = _sequenceIsValid(value);
        break;
    case token_param::FLAGS:
        isValid = _seqFlagsIsValid(value);
        break;
    case token_param::CHR:
        isValid = true;
        break;
    case token_param::SEQ_NAME:
        isValid = true;
        break;
    default:
        break;
    }
    return isValid;
}

void uToken::_validateToken()
{
    try
    {
        std::string str_sequence;
        std::string str_chr = getParam(token_param::CHR);
        std::string str_start_pos = getParam(token_param::START_POS);
        int int_start_pos = atoi(str_start_pos.c_str());
        std::string str_end_pos = getParam(token_param::END_POS);
        int int_end_pos = atoi(str_end_pos.c_str());

        if (isParamSet(token_param::SEQUENCE))str_sequence = getParam(token_param::SEQUENCE);

        /**< Mandatory values: CHR, START_POS and END_POS */
        if (str_chr.size() == 0 || str_start_pos.size() == 0 || str_end_pos.size() == 0)
        {
            std::string error = "CHR, START_POS and END_POS are mandatory.\n";
            error += "CHR: " + str_chr + "\n";
            error += "START_POS: " + str_end_pos + "\n";
            error += "END_POS: " + str_start_pos + "\n";
            _throwInvalidToken(error);
        }
        /**< Validate start/end positions  */
        if (int_start_pos > int_end_pos)
        {
            std::string error = "Invalid START_POS/END_POS values. \n";
            error += "START_POS:" + str_start_pos + ". END_POS: " + str_end_pos + ".";
            _throwInvalidToken(error);
        }
        /**< Validate sequence/phred  */
        if (isParamSet(token_param::PHRED_SCORE))
        {
            std::string str_phred = getParam(token_param::PHRED_SCORE);
            if (str_phred.size() != 0)
            {
                if (str_sequence.size() != str_phred.size())
                {
                    std::stringstream seq_size;
                    seq_size << str_sequence.size();
                    std::stringstream phred_size;
                    phred_size << str_phred.size();
                    std::string error = "Sequence and phred score length does not match.\n";
                    error += "Sequence size: " + seq_size.str() + ". Phred size: " + phred_size.str() + ".";
                    _throwInvalidToken(error);
                }
            }
        }
        /**< Validate sequence/cigar - Sum of lengths of the M/I/S/=/X operations shall equal the length of SEQ. (From SAM specifications) */

       if (isParamSet(token_param::CIGAR))
        {
            std::string str_cigar = getParam(token_param::CIGAR);
            /**< Fetch all numerical values and sum them  */
            size_t cigar_size = 0;
            std::stringstream ss;
            for (size_t i = 0; i < str_cigar.size(); i++)
            {
                if (_isDigit(str_cigar[i]) == true)
                {
                    ss << str_cigar[i];
                }
                else
                {
                    int value;
                    switch(str_cigar[i])
                    {
                    case 'M':
                    case 'I':
                    case 'S':
                    case '=':
                    case 'X':
                        ss >> value;
                        break;
                    default:
                        value = 0;
                        break;
                    }
                    cigar_size += value;
                    ss.clear();
                }

            }
            /**< Check if sequence length and cigar sum match  */
            if (str_sequence.size() != cigar_size)
            {
                std::stringstream seq_size_stream;
                seq_size_stream << str_sequence.size();
                std::stringstream cigar_size_stream;
                cigar_size_stream << cigar_size;
                std::string error = "Sequence length and sum of cigar M/I/S/=/X values do not match.\n";
                error += "Sequence length: " + seq_size_stream.str();
                error += ". Sum of cigar values: " + cigar_size_stream.str() + ".";
                _throwInvalidToken(error);
            }
        }
    }
    catch(invalid_uToken_throw &e)
    {
        throw e;
    }
}

void uToken::_throwInvalidToken(const std::string& baseErrorMessage) const
{
    invalid_uToken_throw e;
    e << invalid_uToken_error("Invalid token. " + baseErrorMessage);
    throw e;
}

/** \brief check if START_POS or END_POS is valid.
 * \param const std::string& value: the value of the pos entry.
 */
bool uToken::_posIsValid(const std::string& value) const
{
    std::stringstream ss;
    ss << value;
    /**< Check if the value is a long int greater than 0 */
    long int pos = 0;
    ss >> pos;
    if (pos <= 0)
    {
        return false;
    }
    /**< Finally, check if there is garbage at the end of value */
    return _isStreamEmpty(ss);
}

/** \brief Check if strand value is valid.
 * \param const std::string& value: the value of the strand entry.
 */
bool uToken::_strandIsValid(const std::string& value) const
{
    /**< Check if size is equal to 1 */
    if (value.size() != 1)
    {
        return false;
    }
    /**< Check if value is either '+' or '-' */
    if (value != "+" && value != "-")
    {
        return false;
    }
    return true;
}
/** \brief Check if map score value is valid (between 0 and 255 incl.).
 * \param const std::string& value: the value of the map score entry.
 */
bool uToken::_mapScoreIsValid(const std::string& value) const
{
    std::stringstream ss;
    ss << value;
    /**< Check if value is between */
    int map = -1;
    ss >> map;
    if (map < 0 || map > 255)
    {
        return false;
    }
    /**< Finally, check if there is garbage at the end of value */
    return _isStreamEmpty(ss);
}

/** \brief Check is sequence is valid.
 * \param const std::string& value: the value of the sequence entry.
 */
bool uToken::_sequenceIsValid(const std::string& value) const
{
    /**< Check if the first value is '*' and the size is 1 */
    if (value[0]== '*' && value.size() > 1)
    {
        return false;
    }
    /**< Otherwise, check if all the characters are in a valid format */
    for (size_t i = 0; i < value.size(); i++)
    {
        switch (value[i])
        {
        case 'a':
            break;
        case 'A':
            break;
        case 'c':
            break;
        case 'C':
            break;
        case 'g':
            break;
        case 'G':
            break;
        case 't':
            break;
        case 'T':
            break;
        case 'n':
            break;
        case 'N':
            break;
        case '.':
            break;
        default:
            return false;
        }
    }
    return true;
}

/** \brief Check if flag value is valid (between 0 and 65235 incl.).
 * \param const std::string& value: the value of the flag entry.
 */
bool uToken::_seqFlagsIsValid(const std::string& value) const
{
    std::stringstream ss;
    ss << value;
    /**< Check if value is between 0 and 65235 incl. */
    int flags = -1;
    ss >> flags;
    if (flags < 0 || flags > 65535)
    {
        return false;
    }
    /**< Finally, check if there is garbage at the end of value */
    return _isStreamEmpty(ss);
}
//TODO extra validation
/** \brief Check if cigar value is valid.
 * \param const std::string& value: the value of the cigar entry.
 */
bool uToken::_cigarIsValid(const std::string& value) const
{
    /**< Check if the first value is '*' and the size is 1 */
    if (value[0] == '*' && value.size() > 1)
    {
        return false;
    }
    /**< First item of value has to be a digit.  */
    if (_isDigit(value[0]) != true)
    {
        return false;
    }
    /**< Make sure there is not two alphabetical character in a row */
    bool lastValue = true;
    for (size_t i = 0; i < value.size(); i++)
    {
        bool currentValue = _isDigit(value[i]);
        if (lastValue == false && currentValue == false)
        {
            return false;
        }
        lastValue = currentValue;
    }
    /**< Must end with alphabetical value */
    if (_isDigit(value[value.size()-1]) == true)
    {
        return false;
    }
    return true;
}

/** \brief Check if a value of a cigar score is a legal character.
 * \param char value: The value to test.
 */
bool uToken::_cigarValueIsValid(char value) const
{
    switch(value)
    {
    case 'M':
        return true;
    case 'I':
        return true;
    case 'D':
        return true;
    case 'N':
        return true;
    case 'S':
        return true;
    case 'H':
        return true;
    case 'P':
        return true;
    case 'X':
        return true;
    case '=':
        return true;
    default:
        return false;
    }
}

/** \brief Check if a istream is empty.
 * \param const std::istream& stream: the istream to check.
 */
bool uToken::_isStreamEmpty(const std::istream& stream) const
{
    if (stream.rdbuf()->in_avail() != 0)
    {
        return false;
    }
    return true;
}

/**< Post processing methods */

/** \brief Sam Flag can contain multiple settings
 *
 * \param value
 * \return void
 *
 */
void uToken::_postProcFlag(const std::string& flag)
{

}
void uToken::_postProcCigar(const std::string& cig)
{
    try
    {
        int curPos=0;
        std::string substr;
        int size=0;
        for (unsigned int i=0; i< cig.size(); i++)
        {
            /**< If isAlpha then check previous numbers */
            if (isalpha(cig.at(i)))
            {
                /**< If a count value */
                char temp;
                temp = cig.at(i);
                if ((temp=='M')||(temp=='I')||(temp=='S')||(temp=='X')||(temp=='+'))
                {
                    substr= cig.substr(curPos, (i-curPos));
                    size+= atoi(substr.c_str());
                }
                curPos=(i+1);
            }
        }
        /**< If END_POS is not set, set it */
        if (!(isParamSet(token_param::END_POS)))
        {
            auto start_pos=utility::stringToInt(getParam(token_param::START_POS));
            _setParam(token_param::END_POS, utility::numberToString(start_pos+(size-1) ));
        }
    }
    catch(uToken_exception_base &e)
    {
        addStringError(e, "Throwing, in _postProccIgar, unable to set EndPOS as START_POS not set");
        throw e;
    }

}

