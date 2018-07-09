struct json_object *root, *data;
 
root = json_object_new_object();
json_object_object_add(root, "name", json_object_new_string("Brian"));
json_object_object_add(root, "sex", json_object_new_int(0));
 
data = json_object_new_object();
json_object_object_add(data, "education", json_object_new_string("master"));
json_object_object_add(data, "profession", json_object_new_string("engineer"));
json_object_object_add(root, "data", data);
 
// Output to string
printf("%s", json_object_to_json_string(root));
 
// Decrease counter and free object
json_object_put(data);
json_object_put(root);
