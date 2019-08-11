
#ifndef _HTTP_SOCKET_H_INCLUDE
#define _HTTP_SOCKET_H_INCLUDE

#include "sockets-config.h"
#include "TcpSocket.h"
#include "Utility.h"

namespace dai {

/**
 * \defgroup http HTTP Sockets
 */


/**
 * HTTP request/response base class.
 * \ingroup http
 */
class HTTPSocket : public TcpSocket
{
    // map to hold http header values.
    using string_m = Utility::ncmap <std::string>;

public:
    HTTPSocket(ISocketHandler&);
    ~HTTPSocket() = default;

    void OnRawData(const char *buf, size_t len);
    void OnLine(const std::string& line);

    /**
     * Callback executes when first line has been received.
     * GetMethod, GetUrl/GetUri, and GetHttpVersion are valid when this callback is executed.
     */
    virtual void OnFirst() = 0;

    /**
     * For each header line this callback is executed.
     * \param key Http header name
     * \param value Http header value
     */
    virtual void OnHeader(const std::string& key, const std::string& value) = 0;

    /**
     * Callback fires when all http headers have been received.
     */
    virtual void OnHeaderComplete() = 0;

    /**
     * Chunk of http body data recevied.
     */
    virtual void OnData(const char *, size_t) = 0;

    /**
     * The full request/response body has been received.
     */
    virtual void OnDataComplete() {}

    /**
     * Get http method from incoming request, ie GET/POST/PUT etc
     */
    const std::string& GetMethod() const;

public:
    /** Get url from request. */
    const std::string& GetUrl() const;

    /** Get part of url before '?' character. */
    const std::string& GetUri() const;

    /** Get part of url after '?' character. */
    const std::string& GetQueryString() const;

    /** Get http version from incoming request/response. */
    const std::string& GetHttpVersion() const;

    /** Get http status from incoming response. */
    const std::string& GetStatus() const;

    /** Get http statustext from incoming response. */
    const std::string& GetStatusText() const;

    /** Incoming header has been identified as a request (method url http_version\r\n). */
    bool IsRequest() const;

    /** Incoming header has been identified as a response (http_version status status_text\r\n). */
    bool IsResponse() const;

    /** Set http version to be used in outgoing request/response. */
    void SetHttpVersion(const std::string& x);

    /** Set http status for outgoing response. */
    void SetStatus(const std::string& x);

    /** Set http statustext for outgoing response. */
    void SetStatusText(const std::string& x);

    /** Add (and replace if exists) http header. */
    void AddResponseHeader(const std::string& x, const std::string& y);

    /** Add (and replace if exists) http header. */
    void AddResponseHeader(const std::string& x, const char *format, ...);

    /** Add http header. */
    void AppendResponseHeader(const std::string& x, const std::string& y);

    /** See if http header 'name' has been set. */
    bool ResponseHeaderIsSet(const std::string& name) const;

    /** Send response prepared with calls to methods SetHttpVersion, SetStatus, SetStatusText,
        and AddResponseHeader. */
    void SendResponse();

    /** Send request prepared with calls to methods SetMethod, SetUrl, SetHttpVersion,
        and AddResponseHeader. */
    void SendRequest();

    /** Implement this to return your own User-agent string. */
    virtual std::string MyUseragent();

    /** Parse url. If protocol is https, EnableSSL() will be called. */
    void url_this(const std::string& url_in,
                  std::string& protocol,
                  std::string& host,
                  port_t& port,
                  std::string& url,
                  std::string& file);

    /**
     * Transfer coding 'chunked'
     */
    bool IsChunked() const
    {
        return m_b_chunked;
    }

protected:
    HTTPSocket(const HTTPSocket& s) : TcpSocket(s) {}

    /**
     * Reset state of socket to sucessfully implement keep-alive.
     */
    virtual void Reset();

    void SetMaxHeaderCount(int x)
    {
        m_max_header_count = x;
    }

    /** Set http method to be used in request. */
    void SetMethod(const std::string& x);

    /** Set url to be used in outgoing request. */
    void SetUrl(const std::string& x);

    /** Now why would I need this when there is a SetUrl method? */
    void SetUri(const std::string& x);

private:
    HTTPSocket& operator=(const HTTPSocket&)
    {
        return *this;
    }

    bool        m_first;
    bool        m_header;
    std::string m_line;
    std::string m_method;
    std::string m_url;
    std::string m_uri;
    std::string m_query_string;
    std::string m_http_version;
    std::string m_status;
    std::string m_status_text;
    bool        m_request;
    bool        m_response;
    string_m    m_response_header;
    size_t      m_body_size_left;
    bool        m_b_http_1_1;
    bool        m_b_keepalive;
    std::list<std::pair<std::string, std::string> > m_response_header_append;
    bool        m_b_chunked;
    size_t      m_chunk_size;
    int         m_chunk_state;
    std::string m_chunk_line;
    int         m_header_count;
    int         m_max_header_count;
};

}//namespace dai

#endif//_HTTP_SOCKET_H_INCLUDE
