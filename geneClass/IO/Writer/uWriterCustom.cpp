#include "uWriterCustom.h"

namespace NGS {

/** \brief Destructor of uWriterCustom
  */
uWriterCustom::~uWriterCustom() {
	if (m_dynamicStream == true) {
		delete m_pOstream;
	}
	m_pOstream = nullptr;
}

/** \brief Initialise the Custom writer with a file name.
  * \param const std::string& filename: name of the file to write output. 
  */
//void uWriterCustom::init(const std::string& filename, const std::vector<std::string>& fieldsNames) {
void uWriterCustom::init(const std::string& filename) {
//	if (fieldsNames.size() == 0) {
//		throw no_fields_names() << string_error("fieldsNames vector is empty");
//	}
	std::filebuf fb;
	fb.open (filename.c_str(),std::ios::out);
	if (!fb.is_open()) {  
		std::string error = "Error opening file: " + filename;
		throw std::runtime_error(error.c_str());
	}
	else {  
		std::ostream* os = new std::ostream(&fb);
		m_pOstream = os;
	}
	m_dynamicStream = true;
//	m_fieldsNames = fieldsNames;
}

/** \brief Initialise the Custom writer with a stream
  * \param std::ostream* os: the stream to save the data to.
  */
//void uWriterCustom::init(std::ostream* os, const std::vector<std::string>& fieldsNames) {
void uWriterCustom::init(std::ostream* os) {
	m_pOstream = os;
}

/** \brief Sets the fields names for the writer.
  * \param const std::vector<std::string>& fieldsNames: Vector containing the name of every fields (columns). In correct order.
  */
//void uWriterCustom::setFieldsNames(const std::vector<std::string> fieldsNames) {
//	if (fieldsNames.size() == 0) {
//		throw no_fields_names() << string_error("fieldsNames vector is empty");
//	}
//	m_fieldsNames = fieldsNames;
//	m_dynamicStream = false;
//}

/** \brief Print the values of a token in Custom format in current file
  * \param const uToken& token: the token to print.
  */
void uWriterCustom::writeToken(const uToken& token) {
	std::vector<std::string> values;
	for (size_t i = 0; i < m_fieldsNames.size(); i++) {
		if (uToken::checkParam(m_fieldsNames[i]) == true) {
			std::stringstream ss;
			ss << m_fieldsNames[i];
			token_param param;
			try {
				ss >> param;
				if (token.isParamSet(param)) {
					values.push_back(token.getParam(param));
				}
				else {
					values.push_back(".");
				}
			}
			catch (invalid_token_param_throw& e) {
				values.push_back(".");
			}
		}
		else {
			values.push_back(".");
		}
		*m_pOstream << values[i];
		if (i != m_fieldsNames.size() - 1) {
			*m_pOstream << '\t';
		}
	}
	*m_pOstream << std::endl;
}

DerivedRegister<uWriterCustom> uWriterCustom::reg("CUSTOM");
} // End of namespace NGS
