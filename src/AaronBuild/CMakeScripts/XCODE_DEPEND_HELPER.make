# DO NOT EDIT
# This makefile makes sure all linkable targets are
# up-to-date with anything they link to
default:
	echo "Do not invoke directly"

# Rules to remove targets that are older than anything to which they
# link.  This forces Xcode to relink the targets from scratch.  It
# does not seem to check these dependencies itself.
PostBuild.ArmorDetect.Debug:
/Users/Aaron/Documents/GitHub/ArmorDetect/src/AaronBuild/Debug/ArmorDetect:\
	/usr/local/lib/libopencv_dnn.3.4.0.dylib\
	/usr/local/lib/libopencv_ml.3.4.0.dylib\
	/usr/local/lib/libopencv_objdetect.3.4.0.dylib\
	/usr/local/lib/libopencv_shape.3.4.0.dylib\
	/usr/local/lib/libopencv_stitching.3.4.0.dylib\
	/usr/local/lib/libopencv_superres.3.4.0.dylib\
	/usr/local/lib/libopencv_videostab.3.4.0.dylib\
	/usr/local/lib/libopencv_calib3d.3.4.0.dylib\
	/usr/local/lib/libopencv_features2d.3.4.0.dylib\
	/usr/local/lib/libopencv_flann.3.4.0.dylib\
	/usr/local/lib/libopencv_highgui.3.4.0.dylib\
	/usr/local/lib/libopencv_photo.3.4.0.dylib\
	/usr/local/lib/libopencv_video.3.4.0.dylib\
	/usr/local/lib/libopencv_videoio.3.4.0.dylib\
	/usr/local/lib/libopencv_imgcodecs.3.4.0.dylib\
	/usr/local/lib/libopencv_imgproc.3.4.0.dylib\
	/usr/local/lib/libopencv_core.3.4.0.dylib
	/bin/rm -f /Users/Aaron/Documents/GitHub/ArmorDetect/src/AaronBuild/Debug/ArmorDetect


PostBuild.ArmorDetect.Release:
/Users/Aaron/Documents/GitHub/ArmorDetect/src/AaronBuild/Release/ArmorDetect:\
	/usr/local/lib/libopencv_dnn.3.4.0.dylib\
	/usr/local/lib/libopencv_ml.3.4.0.dylib\
	/usr/local/lib/libopencv_objdetect.3.4.0.dylib\
	/usr/local/lib/libopencv_shape.3.4.0.dylib\
	/usr/local/lib/libopencv_stitching.3.4.0.dylib\
	/usr/local/lib/libopencv_superres.3.4.0.dylib\
	/usr/local/lib/libopencv_videostab.3.4.0.dylib\
	/usr/local/lib/libopencv_calib3d.3.4.0.dylib\
	/usr/local/lib/libopencv_features2d.3.4.0.dylib\
	/usr/local/lib/libopencv_flann.3.4.0.dylib\
	/usr/local/lib/libopencv_highgui.3.4.0.dylib\
	/usr/local/lib/libopencv_photo.3.4.0.dylib\
	/usr/local/lib/libopencv_video.3.4.0.dylib\
	/usr/local/lib/libopencv_videoio.3.4.0.dylib\
	/usr/local/lib/libopencv_imgcodecs.3.4.0.dylib\
	/usr/local/lib/libopencv_imgproc.3.4.0.dylib\
	/usr/local/lib/libopencv_core.3.4.0.dylib
	/bin/rm -f /Users/Aaron/Documents/GitHub/ArmorDetect/src/AaronBuild/Release/ArmorDetect


PostBuild.ArmorDetect.MinSizeRel:
/Users/Aaron/Documents/GitHub/ArmorDetect/src/AaronBuild/MinSizeRel/ArmorDetect:\
	/usr/local/lib/libopencv_dnn.3.4.0.dylib\
	/usr/local/lib/libopencv_ml.3.4.0.dylib\
	/usr/local/lib/libopencv_objdetect.3.4.0.dylib\
	/usr/local/lib/libopencv_shape.3.4.0.dylib\
	/usr/local/lib/libopencv_stitching.3.4.0.dylib\
	/usr/local/lib/libopencv_superres.3.4.0.dylib\
	/usr/local/lib/libopencv_videostab.3.4.0.dylib\
	/usr/local/lib/libopencv_calib3d.3.4.0.dylib\
	/usr/local/lib/libopencv_features2d.3.4.0.dylib\
	/usr/local/lib/libopencv_flann.3.4.0.dylib\
	/usr/local/lib/libopencv_highgui.3.4.0.dylib\
	/usr/local/lib/libopencv_photo.3.4.0.dylib\
	/usr/local/lib/libopencv_video.3.4.0.dylib\
	/usr/local/lib/libopencv_videoio.3.4.0.dylib\
	/usr/local/lib/libopencv_imgcodecs.3.4.0.dylib\
	/usr/local/lib/libopencv_imgproc.3.4.0.dylib\
	/usr/local/lib/libopencv_core.3.4.0.dylib
	/bin/rm -f /Users/Aaron/Documents/GitHub/ArmorDetect/src/AaronBuild/MinSizeRel/ArmorDetect


PostBuild.ArmorDetect.RelWithDebInfo:
/Users/Aaron/Documents/GitHub/ArmorDetect/src/AaronBuild/RelWithDebInfo/ArmorDetect:\
	/usr/local/lib/libopencv_dnn.3.4.0.dylib\
	/usr/local/lib/libopencv_ml.3.4.0.dylib\
	/usr/local/lib/libopencv_objdetect.3.4.0.dylib\
	/usr/local/lib/libopencv_shape.3.4.0.dylib\
	/usr/local/lib/libopencv_stitching.3.4.0.dylib\
	/usr/local/lib/libopencv_superres.3.4.0.dylib\
	/usr/local/lib/libopencv_videostab.3.4.0.dylib\
	/usr/local/lib/libopencv_calib3d.3.4.0.dylib\
	/usr/local/lib/libopencv_features2d.3.4.0.dylib\
	/usr/local/lib/libopencv_flann.3.4.0.dylib\
	/usr/local/lib/libopencv_highgui.3.4.0.dylib\
	/usr/local/lib/libopencv_photo.3.4.0.dylib\
	/usr/local/lib/libopencv_video.3.4.0.dylib\
	/usr/local/lib/libopencv_videoio.3.4.0.dylib\
	/usr/local/lib/libopencv_imgcodecs.3.4.0.dylib\
	/usr/local/lib/libopencv_imgproc.3.4.0.dylib\
	/usr/local/lib/libopencv_core.3.4.0.dylib
	/bin/rm -f /Users/Aaron/Documents/GitHub/ArmorDetect/src/AaronBuild/RelWithDebInfo/ArmorDetect




# For each target create a dummy ruleso the target does not have to exist
/usr/local/lib/libopencv_calib3d.3.4.0.dylib:
/usr/local/lib/libopencv_core.3.4.0.dylib:
/usr/local/lib/libopencv_dnn.3.4.0.dylib:
/usr/local/lib/libopencv_features2d.3.4.0.dylib:
/usr/local/lib/libopencv_flann.3.4.0.dylib:
/usr/local/lib/libopencv_highgui.3.4.0.dylib:
/usr/local/lib/libopencv_imgcodecs.3.4.0.dylib:
/usr/local/lib/libopencv_imgproc.3.4.0.dylib:
/usr/local/lib/libopencv_ml.3.4.0.dylib:
/usr/local/lib/libopencv_objdetect.3.4.0.dylib:
/usr/local/lib/libopencv_photo.3.4.0.dylib:
/usr/local/lib/libopencv_shape.3.4.0.dylib:
/usr/local/lib/libopencv_stitching.3.4.0.dylib:
/usr/local/lib/libopencv_superres.3.4.0.dylib:
/usr/local/lib/libopencv_video.3.4.0.dylib:
/usr/local/lib/libopencv_videoio.3.4.0.dylib:
/usr/local/lib/libopencv_videostab.3.4.0.dylib:
