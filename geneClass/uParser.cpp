#include "uParser.h"
#include "uGeneException.h"
/** \brief uParser constructor with filename
 * \param std::string filename: Name of the file to load
 * \param file_type type: Currently supported formats: BED, SAM
 */
uParser::uParser(const std::string& filename, file_type type) {
	std::ifstream* ifs = new std::ifstream(filename.c_str(), std::ifstream::in);
	if (!ifs->is_open()) {
		std::string error = "Error opening file: " + filename;
		throw std::runtime_error(error.c_str());
	}
	else {
		m_pIstream = ifs;
	}
	m_fileType = type;
	m_dynamicStream = true;
}

/** \brief uParser constructor with istream
 * \param std::istream* stream: stream to load the data from
 * \param file_type type: Currently supported formats: BED, SAM
 */
uParser::uParser(std::istream* stream, file_type type) {
	m_pIstream = stream;
	m_fileType = type;
	m_dynamicStream = false;
}

/** \brief uParser constructor with custom file type
 * \param const std::vector<string> columnNames: The name of every field in the custom format, in the SAME ORDER as they appear in the file. Must be of the string version of token_param (see uToken.h). Mandatory fields are: CHR, START_POS and END_POS.
 * \param char delimiter: The delimiter between each field.
 */
uParser::uParser(const std::string& filename, const std::vector<std::string>& fieldsNames, char delimiter) {
	/**< Check if filename is valid, then open it */
	std::ifstream* ifs = new std::ifstream(filename.c_str(), std::ifstream::in);
	if (!ifs->is_open()) {
		std::string error = "Error opening file: " + filename;
		throw std::runtime_error(error.c_str());
	}
	else {
		m_pIstream = ifs;
	}
	/**< Check if fields are in a valid format */
	try {
		_customParserValidateFields(fieldsNames);
		_customParserCopyFields(fieldsNames);
	}
	catch(customParser_missing_mandatory_values& e){ 
		throw e;
	}
	/**< Set other parameters */
	m_fileType = file_type::CUSTOM;
	m_delimiter = delimiter;
	m_dynamicStream = true;
}

uParser::uParser(std::istream* stream, const std::vector<std::string>& fieldsNames, char delimiter) {
	/**< Check if fields are in a valid format */
	try {
		_customParserValidateFields(fieldsNames);
		_customParserCopyFields(fieldsNames);
	}
	catch(customParser_missing_mandatory_values& e){ 
		throw e;
	}
	/**< Set other parameters */
	m_pIstream = stream;
	m_fileType = file_type::CUSTOM;
	m_delimiter = delimiter;
	m_dynamicStream = false;
}

/** \brief uParser destructor
 */
uParser::~uParser() {
	if (m_dynamicStream == true) {
		delete m_pIstream;
	}
	m_pIstream = NULL;
}

/** \brief Generic function to fetch an entry from file, will adapt itself to file_type given in constructor
 */
uToken uParser::getNextEntry() {
	try {
		switch(m_fileType) {
		case file_type::BED:
			try {
				uToken token = _getNextEntryBed(); return token;
			}
			catch(invalid_uToken_throw& e) {
				std::string trace = fetchStringError(e);
				#ifdef DEBUG
				std::cerr << "Invalid uToken: " << trace << std::endl;
				#endif
				throw e;
			}
			catch(end_of_file_throw& e) {
				throw e;
			}
			break;
				
		case file_type::SAM:
			try {
				uToken token = _getNextEntrySam(); return token;
			}
			catch(invalid_uToken_throw& e) {
				std::string trace = fetchStringError(e);
				#ifdef DEBUG
				std::cerr << "Invalid uToken: " << trace << std::endl;
				#endif
				throw e;
			}
			break;

		default:
			throw uParser_exception_base()<< string_error("Invalid fileType in getNextEntry case");
			break;
		}
	}
	catch(invalid_uToken_throw &e){
		throw e;
	}
}

/** \brief Specific loader for BED file (See genome.ucsc.edu/FAQ/FAQformat.html#format1 for bed description)
 * \return uToken: If all the parameters in the entry are valid a uToken object is returned.
 */
uToken uParser::_getNextEntryBed() {
	char line[4096];
	if (m_pIstream->getline(line, 4096)) {
		std::stringstream ss;
		ss << line;
		std::string chr;
		std::string start_pos;
		std::string end_pos;
		std::string score;
		std::string seq_name;
		std::string strand;
		std::stringstream token_infos;
		ss >> chr >> start_pos >> end_pos >> seq_name >> score >> strand;
		token_infos << "CHR\t" << chr << "\n";
		token_infos << "START_POS\t" << start_pos << "\n";
		token_infos << "END_POS\t" << end_pos << "\n";
		token_infos << "SEQ_NAME\t" << seq_name << "\n";

		/**< If there was no strand info, we don't add an empty string */
		if (strand.size() != 0) {
			token_infos << "STRAND\t" << strand << "\n";
		}
		try {
			uToken token(token_infos);
			return token;
		}
		catch(invalid_uToken_throw& e) {
			throw e;
		}
	}
	else {
		#ifdef DEBUG
		std::cerr << "Reached end of file." << std::endl;
		#endif
		end_of_file_throw e;
		e << string_error("Reached end of file.");
		throw e;
	}
}
//enum class token_param { CHR, START_POS, END_POS, STRAND, MAP_SCORE, PHRED_SCORE, CIGAR, SEQUENCE, SEQ_NAME, FLAGS };

/** \brief Specific loader for SAM file (See samtools.sourceforge.net for SAM description)
 * \return uToken: If all the parameters in the entry are valid a uToken object is returned.
 */
uToken uParser::_getNextEntrySam() {
	try {
		char line[4096];
		if (m_pIstream->getline(line, 4096)) {
			std::stringstream ss;
			ss << line;

			//String to test for int
			std::string flag;
			std::string start_pos;
			std::string MAPQual;
			std::string pNext;
			std::string Tlen;

			std::string chr;

			std::string end_pos;
			std::string score;
			std::string seq_name;
			std::string qual;
			std::string seq;
			std::string cigar;
			std::string RNext;
			std::stringstream token_infos;

			ss >> seq_name >> flag >> chr >> start_pos >> MAPQual >> cigar>>RNext>>pNext>>Tlen>>seq>>qual;

			token_infos << "CHR\t" << chr << "\n";
			token_infos << "START_POS\t" << start_pos << "\n";
			//token_infos << "END_POS\t" << end_pos << "\n";
			token_infos << "FLAG\t" << flag << "\n";
			token_infos << "SEQ_NAME\t" << seq_name << "\n";
			token_infos << "MAP_SCORE\t" << MAPQual << "\n";
			token_infos << "SEQUENCE\t" << seq << "\n";
			token_infos << "CIGAR\t" << cigar << "\n";
			token_infos << "PHRED_SCORE\t" << qual << "\n";

			uToken token(token_infos);
			return token;
		}
		else {
			#ifdef DEBUG
			std::cerr << "Reached end of file." << std::endl;
			#endif
			end_of_file_throw e;
			e << string_error("Reached end of file.");
			throw e;
		}
	}
	catch(invalid_uToken_throw& e) {
		throw e;
	}
}

void uParser::_customParserValidateFields(const std::vector<std::string>& fieldsNames) {
	/**< Must have at least 2 fields */
	if (fieldsNames.size() < 2) {
		customParser_missing_mandatory_values e;
		e << string_error("Custom file fields description is too short, must have at least 2 values: CHR and START_POS.\n");
		throw e;
	}

	/**< Check if mandatory fields are present */
	std::vector<std::string>::const_iterator it;
	it = find(fieldsNames.begin(), fieldsNames.end(), "CHR");
	if (it == fieldsNames.end()) {
		customParser_missing_mandatory_values e;
		e << string_error("Mandatory field is missing: CHR\n");
		throw e;
	}
	it = find(fieldsNames.begin(), fieldsNames.end(), "START_POS");
	if (it == fieldsNames.end()) {
		customParser_missing_mandatory_values e;
		e << string_error("Mandatory field is missing: START_POS\n");
		throw e;
	}
}

void uParser::_customParserCopyFields(const std::vector<std::string>& fieldsNames) {
	/**< Only copy fields that have a matching token_param value, otherwise add NA */
	for(size_t i = 0; i < fieldsNames.size(); i++) {
		if (uToken::checkParam(fieldsNames[i]) == true) {
			m_customFieldNames.push_back(fieldsNames[i]);
		}
		else {
			m_customFieldNames.push_back("NA");
		}
	}
}
