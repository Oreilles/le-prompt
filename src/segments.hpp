#ifndef MODULES_H
#define MODULES_H


#include "segment.hpp"
#include "prompt_config.hpp"



Segment SegmentUser (PromptConfig p);
Segment SegmentRoot (PromptConfig p);
Segment SegmentPwd  (PromptConfig p);
Segment SegmentHost (PromptConfig p);
Segment SegmentExit (PromptConfig p);
Segment SegmentJobs (PromptConfig p);
Segment SegmentTime (PromptConfig p);
Segment SegmentGit  (PromptConfig p);
Segment SegmentPerms(PromptConfig p);
Segment SegmentSsh  (PromptConfig p);
Segment SegmentAws  (PromptConfig p);
Segment SegmentVirtualEnv(PromptConfig p);


#endif
