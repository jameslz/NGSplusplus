#ifndef UPARSERFACTORY_H_INCLUDED
#define UPARSERFACTORY_H_INCLUDED

#include "uParserBase.h"
#include "uParserBed.h"
#include "uParserGFF.h"
#include "uParserGTF.h"
#include "uParserSam.h"
#include "uParserWig.h"
#include "uParserBedGraph.h"
#include "uParserCustom.h"
namespace NGS{

template<typename T> uParserBase * createT() { return new T; }

struct uParserBaseFactory {
    typedef std::map<std::string, std::function<uParserBase*()> > parser_map_type;
    virtual ~uParserBaseFactory(){};
    static std::shared_ptr<uParserBase> createInstance(std::string const& s) {
        parser_map_type::iterator it = getParserMap()->find(s);

        if(it == getParserMap()->end()){
            throw uParser_exception_base()<<string_error("Asked for unregistered type: "+s+" in Parser, failling");
        }
      //  std::cerr << "Returning" <<std::endl;
        return std::shared_ptr<uParserBase>(it->second());
    }

    static uParserBaseFactory * GetFact()
        {
        static uParserBaseFactory instance;
        return &instance;
        }

    void Register(const std::string &s, std::function<uParserBase*()> pCreatFunc)
    {
            auto it = getParserMap()->find(s);
            if(it == getParserMap()->end())
            {
                 getParserMap()->insert(std::pair<std::string, std::function<uParserBase*() >> (s, pCreatFunc));
            }
                else
            {
                throw uParser_exception_base()<<string_error("Duplicated type registering in Parser, failling\n Type is:"+s+"\n");
            }
    }


    uParserBaseFactory()
    {
        Register("BED", &uParserBed::Create);
        Register("WIG", &uParserWig::Create);
        Register("SAM", &uParserSam::Create);
        Register("GFF", &uParserGFF::Create);
        Register("GTF", &uParserGTF::Create);
        Register("BEDGRAPH", &uParserBedGraph::Create);
        Register("CUSTOM", &uParserCustom::Create);

    }
protected:
    static parser_map_type * mapItem;
    static parser_map_type * getParserMap()
    {
        if(!mapItem)
	{
	    mapItem= new parser_map_type;
	}
        return mapItem;
    };

private:

};
//
//template<typename T>
//struct DerivedParserRegister : uParserBaseFactory
//{
//    ~DerivedParserRegister(){};
//    DerivedParserRegister(std::string const& s)
//    {
//        auto it = getParserMap()->find(s);
//        if(it == getParserMap()->end())
//	{
//             getParserMap()->insert(std::pair<std::string, std::function<uParserBase*() >> (s, &createT<T>));
//        }
//	else
//        {
//            throw uParser_exception_base()<<string_error("Duplicated type registering in Parser, failling\n Type is:"+s+"\n");
//        }
//    }
//};

}
#endif
