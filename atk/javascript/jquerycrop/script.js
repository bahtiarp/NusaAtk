/**
 *
 * Crop Image While Uploading With jQuery
 * 
 * Copyright 2013, Resalat Haque
 * http://www.w3bees.com/
 *
 */

// set info for cropping image using hidden fields

var $jq_1 = jQuery.noConflict();

function setInfo(i, e) {
	$jq_1('#x').val(e.x1);
	$jq_1('#y').val(e.y1);
	$jq_1('#w').val(e.width);
	$jq_1('#h').val(e.height);
}




$jq_1(document).ready(function() {
	var p = $jq_1("#uploadPreview");
	var ratio = $jq_1("#ratio").val();
	
	// prepare instant preview
	$jq_1("#photo").change(function(){
		// fadeOut or hide preview
		p.fadeOut();

		// prepare HTML5 FileReader
		var oFReader = new FileReader();
		oFReader.readAsDataURL(document.getElementById("photo").files[0]);

		oFReader.onload = function (oFREvent) {
	   		p.attr('src', oFREvent.target.result).fadeIn();
		};
	});	

	// implement imgAreaSelect plug in (http://odyniec.net/projects/imgareaselect/)
	$jq_1('img#uploadPreview').imgAreaSelect({
		// set crop ratio (optional)
		aspectRatio: ratio,
		onSelectEnd: setInfo
	});
});