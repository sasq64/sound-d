#ifndef GME_PLAYER_H
#define GME_PLAYER_H

#include "../../chipplugin.h"

namespace chipmachine {

class GMEPlugin : public ChipPlugin {
public:
	virtual std::string name() const override { return "Game Music Engine"; }
	virtual bool canHandle(const std::string &name) override;
	virtual ChipPlayer *fromFile(const std::string &fileName) override;
};

}

#endif // GME_PLAYER_H