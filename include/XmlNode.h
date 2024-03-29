
#ifndef _XML_NODE_H_INCLUDE
#define _XML_NODE_H_INCLUDE

#include "sockets-config.h"

#ifdef ENABLE_XML

#include <string>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <map>

namespace dai {

class XmlDocument;

class XmlNode
{
public:
    XmlNode() {}
    XmlNode(XmlDocument&);
    XmlNode(XmlDocument&, const std::string& nodepath);
    XmlNode(const XmlNode&, const std::string& nodepath);
    XmlNode(xmlDocPtr, xmlNodePtr ptr);
    XmlNode(XmlDocument&, xmlNodePtr ptr);
    ~XmlNode() = default;

    operator xmlNodePtr() const;

    XmlNode operator[](const std::string& name) const;

    operator std::string() const
    {
        return GetContent();
    }

    void operator++() const;

    xmlDocPtr GetDocument() const
    {
        return m_doc;
    }

    bool Valid() const
    {
        return m_current ? true : false;
    }

    /**
     * Assign document root element to 'current node'.
     * \return Current Node
     */
    xmlNodePtr GetRootElement() const;

    /** Return property 'propname' for 'current node'. */
    std::string GetProperty(const std::string& propname) const;

    bool PropertyExists(const std::string& propname) const;

    /**
     * Assign first child node of 'current node' to 'current node'.
     * \return Current Node
     */
    xmlNodePtr GetChildrenNode() const;

    /**
     * Assign next non-blank (xmlIsBlankNode) node of 'current node' to 'current node'.
     * \return Current Node
     */
    xmlNodePtr GetNextNode() const;

    /** Return nodename of 'current node'. */
    const std::string& GetNodeName() const;

    /** Return content of 'current node'. */
    const std::string& GetContent() const;

    /**
     * Sets 'current node' that the following methods act on:
     * GetProperty
     * GetNodeName
     * GetContent
     * GetNodeNs
     * GetNodeNsPrefix
     * GetNodeNsHref
    */
    void SetCurrent(xmlNodePtr p) const
    {
        m_current = p;
    }

    /**
     * \return Namespace of 'current node'.
     */
    xmlNsPtr GetNodeNs() const;

    /** \return Namespace prefix of 'current node'. */
    const std::string& GetNodeNsPrefix() const;

    /** \return Namespace href of 'current node'. */
    const std::string& GetNodeNsHref() const;

    /**
     * Search element named 'name' in Root element child nodes.
     * Set 'current node' to Root element.
     */
    xmlNodePtr GetFirstElement(const std::string& name) const;

    /**
     * Search element named 'name' in 'parent' element child nodes.
     * Set 'current node' to 'parent'.
     */
    xmlNodePtr GetFirstElement(xmlNodePtr parent, const std::string& name) const;

    /**
     * Find next element named 'name' on same level as 'node'.
     * Set 'current node' to 'p'.
     */
    xmlNodePtr GetNextElement(xmlNodePtr node, const std::string& name) const;

    /** Check that child element with name 'name' exists. */
    bool Exists(const std::string& name) const;

    /** [prefix] = href */
    std::map<std::string, std::string> GetNsMap() const;

    /** [href] = prefix */
    std::map<std::string, std::string> GetNsMapRe() const;

    const std::string FindProperty(const std::string& propname, bool climb = false) const;

private:
    xmlDocPtr           m_doc;
    mutable xmlNodePtr  m_current;
    mutable std::string m_current_name;
    mutable std::string m_ns_prefix;
    mutable std::string m_ns_href;
    mutable std::string m_content;
    mutable std::string m_lookup_name;
};

}//namespace dai

#endif//ENABLE_XML

#endif//_XML_NODE_H_INCLUDE
