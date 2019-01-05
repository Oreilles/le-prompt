#include "prompt.hpp"


Prompt::Prompt(PromptOpt options):options(options)
{
    segments = {
        {"user", new SegmentUser(&this->options)},
        {"root", new SegmentRoot(&this->options)},
        {"pwd",  new SegmentPwd(&this->options)},
        {"exit", new SegmentExit(&this->options)},
        {"git",  new SegmentGit(&this->options)},
        {"host", new SegmentHost(&this->options)},
    };
    threads.reserve(segments.size());
}


ThreadedSegment* Prompt::getSegmentByName(std::string str)
{
    try
    {
        return segments.at(str);
    }
    catch (...)
    {
        return NULL;
    }
}


void Prompt::parseSegments(std::string str)
{
    std::string buff;
    std::stringstream ss(str);
    while(std::getline(ss, buff, ','))
        if (ThreadedSegment *s = getSegmentByName(buff)){
            appendSegment(s);
        }
}


void Prompt::appendSegment(ThreadedSegment *s){
    s->init();
    threads.push_back(s);
};



void Prompt::print()
{
    for (auto &thread : threads){
        if (Segment *s = thread->get()){
            printSegment(*s);
        }
    }
    reset();
}


void Prompt::printSegment(Segment s)
{
    if (s.style.bg == prevColor)
    {
        length += strlen(options.symbols->SeparatorThin);
        setFg(s.style.fg);
        printf("%s", options.symbols->SeparatorThin);
    }
    else
    if (prevColor != -1)
    {
        length += strlen(options.symbols->Separator);
        setBg(s.style.bg);
        setFg(prevColor);
        printf("%s", options.symbols->Separator);
    }
    setBg(s.style.bg);
    setFg(s.style.fg);
    printf(" %s ",s.content);
    length += strlen(s.content) + 2;
    prevColor = s.style.bg;
}


void Prompt::reset(){
    resetStyle();
    setFg(prevColor);
    printf("%s  ", options.symbols->Separator);
    resetStyle();
}