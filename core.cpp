#include "tracelib.h"
#include "configuration.h"

using namespace Tracelib;
using namespace std;

template <class Iterator>
void deleteRange( Iterator begin, Iterator end )
{
    while ( begin != end ) delete *begin++;
}

Output::Output()
{
}

Output::~Output()
{
}

Serializer::Serializer()
{
}

Serializer::~Serializer()
{
}

Filter::Filter()
{
}

Filter::~Filter()
{
}

vector<AbstractVariableConverter *> &Tracelib::operator<<( vector<AbstractVariableConverter *> &v,
                                                 AbstractVariableConverter *c )
{
    v.push_back( c );
    return v;
}

Trace::Trace()
    : m_serializer( 0 ),
    m_output( 0 ),
    m_filter( 0 ),
    m_configuration( new Configuration )
{
    m_filter = m_configuration->configuredFilter();
    m_serializer = m_configuration->configuredSerializer();
}

Trace::~Trace()
{
    delete m_serializer;
    delete m_output;
    delete m_filter;
    delete m_configuration;
}

void Trace::reconsiderTracePoint( TracePoint *tracePoint ) const
{
    tracePoint->active = !m_filter || m_filter->acceptsTracePoint( tracePoint );
    tracePoint->backtracesEnabled = false;
    tracePoint->variableSnapshotEnabled = true;
    tracePoint->lastUsedConfiguration = m_configuration;
}

void Trace::addEntry( const TraceEntry &entry )
{
    if ( !m_serializer || !m_output ) {
        return;
    }

    if ( !m_output->canWrite() ) {
        return;
    }

    vector<char> data = m_serializer->serialize( entry );
    if ( !data.empty() ) {
        m_output->write( data );
    }
}

void Trace::setSerializer( Serializer *serializer )
{
    delete m_serializer;
    m_serializer = serializer;
}

void Trace::setOutput( Output *output )
{
    delete m_output;
    m_output = output;
}

void Trace::setFilter( Filter *filter )
{
    delete m_filter;
    m_filter = filter;
}

static Trace *g_activeTrace = 0;

namespace Tracelib
{

Trace *getActiveTrace()
{
    return g_activeTrace;
}

void setActiveTrace( Trace *trace )
{
    g_activeTrace = trace;
}

}

