#pragma once

struct AIReaction
{
	int32 ReactionEnumType {0};
	bool Consumed {false};
	int64_t StartTime {0};
	int64_t LifeTimeMs {0};
};
