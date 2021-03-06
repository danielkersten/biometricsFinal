# Andres Mejia, Christopher O'Connell
# CS667
# Term Project
# 12/05/2012

DEBUG ?= yes
WERROR ?= yes

CXX = g++
CXXFLAGS = -Wall -Wextra
ifeq ($(WERROR),yes)
CXXFLAGS += -Werror
endif
ifeq ($(DEBUG),yes)
CXXFLAGS += -D_DEBUG -g3 -O0
else
CXXFLAGS += -g0 -O3
endif

LIBS = \
  -lopencv_calib3d \
  -lopencv_contrib \
  -lopencv_core \
  -lopencv_features2d \
  -lopencv_flann \
  -lopencv_highgui \
  -lopencv_imgproc \
  -lopencv_legacy \
  -lopencv_ml \
  -lopencv_objdetect \
  -lopencv_photo \
  -lopencv_stitching \
  -lopencv_ts \
  -lopencv_video \
  -lopencv_videostab

DEPS = \
  CameraCapture.hpp \
  options.hpp

SRCS = \
  CameraCapture.cpp \
  main.cpp \
  options.cpp

OBJS = $(filter %.o,$(SRCS:.cpp=.o))

camCapture: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o camCapture
