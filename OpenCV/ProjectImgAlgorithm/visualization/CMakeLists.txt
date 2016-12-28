ADD_EXECUTABLE( viewer viewer.cpp )
TARGET_LINK_LIBRARIES( viewer ${OpenCV_LIBS} )

ADD_EXECUTABLE( getFrames getFrames.cpp )
TARGET_LINK_LIBRARIES( getFrames ${OpenCV_LIBS} )

ADD_EXECUTABLE( cars findCars.cpp )
TARGET_LINK_LIBRARIES( cars ${OpenCV_LIBS} )

ADD_EXECUTABLE( cars2 findCarsContours.cpp )
TARGET_LINK_LIBRARIES( cars2 ${OpenCV_LIBS} )

ADD_EXECUTABLE( optflow opticalFlow.cpp )
TARGET_LINK_LIBRARIES( optflow ${OpenCV_LIBS} )

#Foi adicionada a libraria restclient-cpp para que o roads possa fazer uso do restclient
ADD_EXECUTABLE( roads Roads.cpp )
TARGET_LINK_LIBRARIES( roads ${OpenCV_LIBS} restclient-cpp)

ADD_EXECUTABLE( roadsLib RoadsLib.cpp )
TARGET_LINK_LIBRARIES( roadsLib ${OpenCV_LIBS} )

ADD_EXECUTABLE( calibRoad calibrateRoad.cpp )
TARGET_LINK_LIBRARIES( calibRoad ${OpenCV_LIBS} )



