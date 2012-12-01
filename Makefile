# Christopher O'Connell, Andres Mejia
# CS667
# Term Project

DEBUG ?= yes

CXX = g++
CXXFLAGS = -Wall -Wextra -Werror
ifeq ($(DEBUG),yes)
CXXFLAGS += -g3 -O0
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

# NOTE: These libraries most likely are not needed.
# -lopencv_gpu.so
# -lopencv_nonfree.so

DEPS = \
  CameraCapture.h \
  options.hpp

SRCS = \
  CameraCapture.cpp \
  camCapture.cpp \
  options.cpp

OBJS = $(filter %.o,$(SRCS:.cpp=.o))

camCapture: $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

%.o: %.cpp $(DEPS)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

.PHONY: clean
clean:
	rm -rf *.o camCapture
