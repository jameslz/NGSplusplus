#include "uWriter.h"

namespace NGS {

uWriter::uWriter(const std::string& filename, const std::string& type) {
	uWriterBaseFactory myFactory;
	m_pWriterBase = myFact.createInstance(type);
	m_pWriterBase->init(filename);
}

uWriter::uWriter(std::ostream* os, const std::string& type) {
	uWriterBaseFactory myFactory;
	m_pWriterBase = myFact.createInstance(type);
	m_pWriterBase->init(filename);
}

void uWriter::writeToken(const uToken& token) {
	m_pWriterBase->writeToken(token);
}

} // End of namespace NGS