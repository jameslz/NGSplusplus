#include "uHeader.h"
namespace NGS {
/** \brief If the param exist, it returns it's value in string format. Otherwise returns param_not_found error.
 * \param header_param name the name of the param from header_param strongly type enum types.
 */

 uHeader::uHeader(){

/**< Register our functions */
    /**< Validate functions */
    validate_func_map[header_param::CHR]= &uHeader::_validateChrList;
    validate_func_map[header_param::CHR_SIZE]=&uHeader::_validateChrSize;
    /**< Post processing function */
    post_func_map[header_param::CHR]=&uHeader::_noPost;
    post_func_map[header_param::CHR_SIZE]=&uHeader::_noPost;

 }

std::vector<std::string> uHeader::getParamVector(header_param name) const{
	if(isParamSet(name)) {
		return (m_params.find(name)->second);
	}
	else {
		param_not_found e;
		std::string error = "Tried to getParam that is not set: " + _convertHeaderParamToString(name) + "\n";
		e << string_error(error);
		e << string_error(_convertHeaderParamToString(name));
		throw e;
	}
}

std::string uHeader::getParam(header_param name) const{
	if(isParamSet(name)) {
		return (m_params.find(name)->second.at(0));
	}
	else {
		param_not_found e;
		std::string error = "Tried to getParam that is not set: " + _convertHeaderParamToString(name) + "\n";
		e << string_error(error);
		e << string_error(_convertHeaderParamToString(name));
		throw e;
	}
}

bool uHeader::_validateChrSize(const std::string& sizeString)const{
    try {
        int value = std::stoi(sizeString);
        /**< Chr size must be above 0 */
        if (value <= 0)
            return false;
        return true;
    }
    catch(...){
        return false;
    }
};


bool uHeader::_validateChrList(const std::string& chrString)const
{
    /**< Make sur our name is unique */
    if (isParamSet(header_param::CHR))
    {
       auto listVector = getParamVector(header_param::CHR);
       auto it = find (listVector.begin(), listVector.end(), chrString);
       if (it!=listVector.end())
        return false;
    }

    return true;
}
 //auto begin()->decltype(ExpMap.begin()){return ExpMap.begin();};
//auto uHeader::getParam(header_param name)->( std::tuple_element<0,  m_params.find(name)->second>)
//{
//    return std::get<0>(m_params.find(name)->second)
//}

/** \brief To copy a uHeader object.
 */
uHeader& uHeader::operator=(uHeader const& assign_from) {
	if (this == &assign_from) return *this;
	m_params = assign_from.m_params;
	m_unformatedHeader = assign_from.m_unformatedHeader;
	return *this;
}

/** \brief Set the value of a param
 * \param const header_param& name: the name of the param. Must be a header_param type.
 * \param const std::string& value: the value of the param in string format.
 */
void uHeader::_setParam(const header_param& name, const std::string& value){
	// TODO: Valiation and post-process of param?
	if (m_params[name].size()==0)
          m_params[name].resize(1);
    if (_validateParam(name,value)==false){
            std::string errMsg;
            std::string strName; strName << name;
            errMsg = ("Invalid header, type = "+  strName +" value = " + value+"\n");
            throw invalid_header_param_throw()<<string_error(errMsg);
        }

    m_params[name].at(0) = value;
}

/** \brief Add a param to the vector of that param
 * \param const header_param& name: the name of the param. Must be a header_param type.
 * \param const std::string& value: the value of the param in string format.
 */
void uHeader::_addToParam(const header_param& name, const std::string& value){
	// TODO: Valiation and post-process of param?
	std::string strName; strName << name;
	  if (_validateParam(name,value)==false){
            std::string errMsg;
            std::string strName; strName << name;
            errMsg = ("Invalid header, type = "+  strName +" value = " + value+"\n");
            throw invalid_header_param_throw()<<string_error(errMsg);
        }
    m_params[name].push_back(value);
}


/** \brief Utility function to convert an header_param type to it's string counterpart.
 * \param const header_param& header: the header_param type to convert to string.
 */
std::string uHeader::_convertHeaderParamToString(const header_param& header) const {

	std::stringstream ss;
	ss << header;
	return ss.str();
	return "";
}

/** \brief Operate any necessary post_processing on our values
 *
 * \param name header_param& Type of parameter
 * \param value const std::string& Value of parameter
 * \return void
 *
 */
void uHeader::_postProcessParam(const header_param& name, std::string& value) {
    if (post_func_map.count(name))
        post_func_map[name](this,value);
}

/** \brief Call the associated validation function
 *
 * \param name const token_param&
 * \param value const std::string&a
 * \return bool
 *
 */
bool uHeader::_validateParam(header_param name, const std::string& value) {
   // std::cout << "validating call" <<std::endl;
   if (validate_func_map.count(name))
        return validate_func_map[name](this,value);
}

} // End of namespace NGS
