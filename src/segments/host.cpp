#define HOSTNAME_MAX 256

#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "../modules.hpp"



void
SegmentHost::make()
{
    char* hostname = (char*)malloc(HOSTNAME_MAX);
    gethostname(hostname, HOSTNAME_MAX);
    segment.content = hostname;
    segment.content.erase(segment.content.find('.'));
    segment.style = opt.theme.hostname;
};