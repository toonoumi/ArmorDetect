# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.ArmorDetect.Debug:
/Users/jasonlu/Documents/Projects/ArmorDetect/build/Debug/ArmorDetect:\
	/usr/local/lib/libopencv_dnn.3.3.0.dylib\
	/usr/local/lib/libopencv_ml.3.3.0.dylib\
	/usr/local/lib/libopencv_objdetect.3.3.0.dylib\
	/usr/local/lib/libopencv_shape.3.3.0.dylib\
	/usr/local/lib/libopencv_stitching.3.3.0.dylib\
	/usr/local/lib/libopencv_superres.3.3.0.dylib\
	/usr/local/lib/libopencv_videostab.3.3.0.dylib\
	/usr/local/lib/libopencv_calib3d.3.3.0.dylib\
	/usr/local/lib/libopencv_features2d.3.3.0.dylib\
	/usr/local/lib/libopencv_flann.3.3.0.dylib\
	/usr/local/lib/libopencv_highgui.3.3.0.dylib\
	/usr/local/lib/libopencv_photo.3.3.0.dylib\
	/usr/local/lib/libopencv_video.3.3.0.dylib\
	/usr/local/lib/libopencv_videoio.3.3.0.dylib\
	/usr/local/lib/libopencv_imgcodecs.3.3.0.dylib\
	/usr/local/lib/libopencv_imgproc.3.3.0.dylib\
	/usr/local/lib/libopencv_core.3.3.0.dylib
	/bin/rm -f /Users/jasonlu/Documents/Projects/ArmorDetect/build/Debug/ArmorDetect


PostBuild.ArmorDetect.Release:
/Users/jasonlu/Documents/Projects/ArmorDetect/build/Release/ArmorDetect:\
	/usr/local/lib/libopencv_dnn.3.3.0.dylib\
	/usr/local/lib/libopencv_ml.3.3.0.dylib\
	/usr/local/lib/libopencv_objdetect.3.3.0.dylib\
	/usr/local/lib/libopencv_shape.3.3.0.dylib\
	/usr/local/lib/libopencv_stitching.3.3.0.dylib\
	/usr/local/lib/libopencv_superres.3.3.0.dylib\
	/usr/local/lib/libopencv_videostab.3.3.0.dylib\
	/usr/local/lib/libopencv_calib3d.3.3.0.dylib\
	/usr/local/lib/libopencv_features2d.3.3.0.dylib\
	/usr/local/lib/libopencv_flann.3.3.0.dylib\
	/usr/local/lib/libopencv_highgui.3.3.0.dylib\
	/usr/local/lib/libopencv_photo.3.3.0.dylib\
	/usr/local/lib/libopencv_video.3.3.0.dylib\
	/usr/local/lib/libopencv_videoio.3.3.0.dylib\
	/usr/local/lib/libopencv_imgcodecs.3.3.0.dylib\
	/usr/local/lib/libopencv_imgproc.3.3.0.dylib\
	/usr/local/lib/libopencv_core.3.3.0.dylib
	/bin/rm -f /Users/jasonlu/Documents/Projects/ArmorDetect/build/Release/ArmorDetect


PostBuild.ArmorDetect.MinSizeRel:
/Users/jasonlu/Documents/Projects/ArmorDetect/build/MinSizeRel/ArmorDetect:\
	/usr/local/lib/libopencv_dnn.3.3.0.dylib\
	/usr/local/lib/libopencv_ml.3.3.0.dylib\
	/usr/local/lib/libopencv_objdetect.3.3.0.dylib\
	/usr/local/lib/libopencv_shape.3.3.0.dylib\
	/usr/local/lib/libopencv_stitching.3.3.0.dylib\
	/usr/local/lib/libopencv_superres.3.3.0.dylib\
	/usr/local/lib/libopencv_videostab.3.3.0.dylib\
	/usr/local/lib/libopencv_calib3d.3.3.0.dylib\
	/usr/local/lib/libopencv_features2d.3.3.0.dylib\
	/usr/local/lib/libopencv_flann.3.3.0.dylib\
	/usr/local/lib/libopencv_highgui.3.3.0.dylib\
	/usr/local/lib/libopencv_photo.3.3.0.dylib\
	/usr/local/lib/libopencv_video.3.3.0.dylib\
	/usr/local/lib/libopencv_videoio.3.3.0.dylib\
	/usr/local/lib/libopencv_imgcodecs.3.3.0.dylib\
	/usr/local/lib/libopencv_imgproc.3.3.0.dylib\
	/usr/local/lib/libopencv_core.3.3.0.dylib
	/bin/rm -f /Users/jasonlu/Documents/Projects/ArmorDetect/build/MinSizeRel/ArmorDetect


PostBuild.ArmorDetect.RelWithDebInfo:
/Users/jasonlu/Documents/Projects/ArmorDetect/build/RelWithDebInfo/ArmorDetect:\
	/usr/local/lib/libopencv_dnn.3.3.0.dylib\
	/usr/local/lib/libopencv_ml.3.3.0.dylib\
	/usr/local/lib/libopencv_objdetect.3.3.0.dylib\
	/usr/local/lib/libopencv_shape.3.3.0.dylib\
	/usr/local/lib/libopencv_stitching.3.3.0.dylib\
	/usr/local/lib/libopencv_superres.3.3.0.dylib\
	/usr/local/lib/libopencv_videostab.3.3.0.dylib\
	/usr/local/lib/libopencv_calib3d.3.3.0.dylib\
	/usr/local/lib/libopencv_features2d.3.3.0.dylib\
	/usr/local/lib/libopencv_flann.3.3.0.dylib\
	/usr/local/lib/libopencv_highgui.3.3.0.dylib\
	/usr/local/lib/libopencv_photo.3.3.0.dylib\
	/usr/local/lib/libopencv_video.3.3.0.dylib\
	/usr/local/lib/libopencv_videoio.3.3.0.dylib\
	/usr/local/lib/libopencv_imgcodecs.3.3.0.dylib\
	/usr/local/lib/libopencv_imgproc.3.3.0.dylib\
	/usr/local/lib/libopencv_core.3.3.0.dylib
	/bin/rm -f /Users/jasonlu/Documents/Projects/ArmorDetect/build/RelWithDebInfo/ArmorDetect




# For each target create a dummy ruleso the target does not have to exist
/usr/local/lib/libopencv_calib3d.3.3.0.dylib:
/usr/local/lib/libopencv_core.3.3.0.dylib:
/usr/local/lib/libopencv_dnn.3.3.0.dylib:
/usr/local/lib/libopencv_features2d.3.3.0.dylib:
/usr/local/lib/libopencv_flann.3.3.0.dylib:
/usr/local/lib/libopencv_highgui.3.3.0.dylib:
/usr/local/lib/libopencv_imgcodecs.3.3.0.dylib:
/usr/local/lib/libopencv_imgproc.3.3.0.dylib:
/usr/local/lib/libopencv_ml.3.3.0.dylib:
/usr/local/lib/libopencv_objdetect.3.3.0.dylib:
/usr/local/lib/libopencv_photo.3.3.0.dylib:
/usr/local/lib/libopencv_shape.3.3.0.dylib:
/usr/local/lib/libopencv_stitching.3.3.0.dylib:
/usr/local/lib/libopencv_superres.3.3.0.dylib:
/usr/local/lib/libopencv_video.3.3.0.dylib:
/usr/local/lib/libopencv_videoio.3.3.0.dylib:
/usr/local/lib/libopencv_videostab.3.3.0.dylib:
