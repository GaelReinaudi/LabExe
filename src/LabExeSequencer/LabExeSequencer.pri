HEADERS += ./Model/GEventGraphicsLine.h \
    ./GEventManager.h \
    ./GSequencer.h \
    ./GSequenceView.h \
    ./GSequencingBench.h \
    ./labexesequencer.h \
    ./labexesequencer_global.h \
    ./Model/GSeqModel.h \
    ./Model/GSequenceEventItem.h \
    ./GChannel.h \
	./GChannelGraphicsItem.h \
	./GSequencerWidget.h \
    ./Sequence/GEventNode.h \
    ./Sequence/GEvScene.h \
    ./Sequence/GSynchEventGraphicsItem.h \
    ./Sequence/GSequence.h \
    ./Sequence/GSequenceGraphicsItem.h \
    ./Sequence/GSynchEvent.h
SOURCES += ./Model/GEventGraphicsLine.cpp \
    ./GEventManager.cpp \
    ./GSequencer.cpp \
    ./GSequenceView.cpp \
    ./GSequencingBench.cpp \
    ./labexesequencer.cpp \
    ./Model/GSeqModel.cpp \
    ./Model/GSequenceEventItem.cpp \
    ./GChannel.cpp \
	./GChannelGraphicsItem.cpp \
	./GSequencerWidget.cpp \
    ./Sequence/GEventNode.cpp \
    ./Sequence/GEvScene.cpp \
    ./Sequence/GSynchEventGraphicsItem.cpp \
    ./Sequence/GSequence.cpp \
    ./Sequence/GSequenceGraphicsItem.cpp \
    ./Sequence/GSynchEvent.cpp
FORMS += ./GSequencerWidget.ui

OTHER_FILES += \
    QML/sequence.qml
