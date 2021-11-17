
function getResults () {
	var radios = document. getElementsByName ("radio");
	for (var i = 0; i < radios. length; i ++) {
	   if (radios [i]. checked) {
	      var s = "Service " + radios [i]. value;
	      document. getElementById ("selectedService"). innerHTML = s;
	      break;
	   }
	}
}

function display_ct () {
	var dt = new Date ();
	document.getElementById ("datetime"). innerHTML = dt. toLocaleTimeString ();
}

var choices = { channel_5A  : "5A",
	        channel_5B  : "5B",
	        channel_5C  : "5C",
	        channel_5D  : "5D",
	        channel_6A  : "6A",
	        channel_6B  : "6B",
	        channel_6C  : "6C",
	        channel_6D  : "6D",
	        channel_7A  : "7A",
	        channel_7B  : "7B",
	        channel_7C  : "7C",
	        channel_7D  : "7D",
	        channel_8A  : "8A",
	        channel_8B  : "8B",
	        channel_8C  : "8C",
	        channel_8D  : "8D",
	        channel_9A  : "9A",
	        channel_9B  : "9B",
	        channel_9C  : "9C",
	        channel_9D  : "9D",
	        channel_10A : "10A",
	        channel_10B : "10B",
	        channel_10C : "10C",
	        channel_10D : "10D",
	        channel_11A : "11A",
	        channel_11B : "11B",
	        channel_11C : "11C",
	        channel_11D : "11D",
	        channel_12A : "12A",
	        channel_12B : "12B",
	        channel_12C : "12C",
	        channel_12D : "12D",
	        channel_13A : "13A",
	        channel_13B : "13B",
	        channel_13C : "13C",
	        channel_13D : "13D",
	        channel_13E : "13E",
	        channel_13F : "13F"
};

function setupChannels () {
var selector	= document. getElementById ("Channels");
	for (index in choices) {
	   selector. options [selector. options. length] =
	                                 new Option (choices [index], index);
	}
}

function setupServices (serviceList) {
	var form	= document. createElement ("form"); 
	var br		= document. createElement ("br");
	form. setAttribute ("method", "post"); 
//	form. setAttribute ("method", "GET"); 
	form. setAttribute ("action", "/select-service"); // / ervoor
	form. setAttribute ("onclick", "getResults ()");

	for (var i = 0; i < serviceList. length; i ++) {
	   var r	= document. createElement ('input');
	   r . setAttribute ("type", "radio");
	   r. setAttribute ("name", 'radio');
	   r. setAttribute ("value", serviceList [i]);
	   if (i == 0)
	      r. setAttribute ("checked", "checked");
	   var label	= document. createElement ("label");
	   label. appendChild (document. createTextNode (serviceList [i]));
	   form. appendChild (r);  
	   form. appendChild (label);
	   form. appendChild(br.cloneNode());  
	}

	var s = document. createElement ("input"); 
	s. setAttribute ("type", "submit"); 
	s. setAttribute ("value", "select Service"); 
// Append the submit button to the form 
	form. appendChild (s);  
	document. getElementById ("serviceSection"). appendChild (form);
}

function setupDynamicLabel (s) {
	document. getElementById ("dynamicLabel"). innerHTML = s;
}

function	setEnsembleName	(ensemble) {
	document. getElementById ("ensembleName"). innerHTML = ensemble;
}

function	updater	() {
	document. form1. submit ();
}

function	setupGainValues	(deviceGain, deviceAGC, soundGain) {
	document. ifgain. value = deviceGain;
	document. autogain. value = deviceAGC;
	document. audiogain. value = soundGain;
}

