#include "uBasicNGS.h"
#include "uTags.h"
#include "uRegion.h"
namespace NGS {

    /** \brief Constructor taking a uTags as parameter.
     * \param uTags p_tags: the tag to add to create the uBasicNGS object from.
     */
    uBasicNGS::uBasicNGS(uTags p_tags)
        : uGenericNGS(p_tags.getChr(),p_tags.getStart(), p_tags.getEnd(), p_tags.getStrand())
    {
        setScoreVector(p_tags.getScoreVector());
    }

    /** \brief Constructor taking a uRegion as parameter.
     * \param uRegion p_region: the region to add to create the uBasicNGS object from.
     */
    uBasicNGS::uBasicNGS(uRegion p_region)
        : uGenericNGS(p_region.getChr(),p_region.getStart(), p_region.getEnd(), p_region.getStrand())
    {
        setScoreVector(p_region.getScoreVector());
    }

    /** \brief Checks if the content of elements are identical
     *
     * \param pCompared const &_SELF_ Item to compare
     * \return bool True if identical
     *
     */

    bool uBasicNGS::isEqual(const uBasicNGS  &  pCompared) const{

        return ((this->getChr()==pCompared.getChr())&&
                (this->getStrand()==pCompared.getStrand())&&
                (this->getStart()==pCompared.getStart())&&
                (this->getEnd()==pCompared.getEnd())&&
                (this->getScoreVector()==pCompared.getScoreVector()));
    }


    uBasicNGS uBasicNGS::getCopy()const{
        uBasicNGS returnCopy = *this;
        return returnCopy;
    }

/**< uBasicNGSChrom */

    /** \brief Copy constructor
     * \param const uBasicNGSChrom& initFrom: the uBasicNGSChrom to copy.
     */
    uBasicNGSChrom::uBasicNGSChrom(const uBasicNGSChrom& initFrom)
    {
        VecSites=initFrom.returnVecData();
        chr= initFrom.getChr();
        m_isSorted=initFrom.m_isSorted;
        sortGetStart=initFrom.sortGetStart;
        sortGetEnd=initFrom.sortGetEnd;
        m_comptFunc=initFrom.m_comptFunc;
        chromSize=initFrom.chromSize;
    }

    /** \brief Assignment operator overload to copy a uBagicNGSChrom.
     * \param const uBasicNGSChrom& copFrom: the uBasicNGSChrom to copy.
     */
    uBasicNGSChrom& uBasicNGSChrom::operator=(const uBasicNGSChrom& copFrom)
    {
        VecSites=copFrom.returnVecData();
        chr= copFrom.getChr();
        m_isSorted=copFrom.m_isSorted;
        sortGetStart=copFrom.sortGetStart;
        sortGetEnd=copFrom.sortGetEnd;
        m_comptFunc=copFrom.m_comptFunc;
        chromSize=copFrom.chromSize;

        return *this;
    }


    uBasicNGSChrom uBasicNGSChrom::getCopy()const
    {
        uBasicNGSChrom returnCopy = *this;
        return returnCopy;
    }


}
