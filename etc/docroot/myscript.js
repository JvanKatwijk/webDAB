
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

function setupChannels () {
var selector	= document. getElementById ("Channels");
	for (index in choices) {
	   selector. options [selector. options. length] =
	                                 new Option (choices [index], index);
	}
}

function setupServices (serviceList, action) {
	var form	= document. createElement ("form"); 
	var br		= document. createElement ("br");
	form. setAttribute ("method", "post"); 
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
	   form.appendChild(br.cloneNode());  
	}

	var s = document.createElement("input"); 
	s. setAttribute ("type", "submit"); 
	s. setAttribute ("value", "select Service"); 
// Append the submit button to the form 
	form. appendChild (s);  
	document. getElementById ("serviceSection"). appendChild (form);
}

function setupServiceData (s) {
	document.getElementById ("servicedata"). innerHTML = s;
}

function setupDynamicLabel (s) {
	document. getElementById ("dynamicLabel"). innerHTML = s;
}

