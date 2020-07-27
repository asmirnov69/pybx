request
=======
message-type: method-call
message-id: <uuid>
method-signature: <string>
object-id: <uuid>
args: <json>

response
========
message-type: method-call-return
message-id: <uuid>
orig-message-id: <uuid>
retval: <json>

exception
=========
message-type: method-call-exception
message-id: <uuid>
orig-message-id: <uuid>
remote-exception-text: <string>
