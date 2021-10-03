# Modificaciones a CPython

## Cambiando la versión

Si miras el archivo `Include/patchlevel.h` podrás encontrar
la constante llamada `PY_VERSION`.
Actualmente (03.10.2021) la versión se define:

```
#define PY_VERSION              "3.11.0a0"
```

si la cambias y compilas CPython, será la versión que muestra
el intérprete, por ejemplo:

```
#define PY_VERSION              "4.0.0a0 versión en Español"
```

## Cambios de tipos y métodos

Los tipos en Python funcionan siguiendo una estructura
determinada de ir:

* declarar funciones, y
* agregarlas a una estructura de funciones para el tipo

En las siguientes secciones verás los ejemplos
utilizados en la charla.

### Sets

Para que la función `.addplease` acepte un set en vez de
solo un elemento, solo copiamos el contenido de la función
`set.update` que ya lo hace. Agregamos esta línea:

```
{"addplease",             (PyCFunction)set_update,           METH_VARARGS,
```
justo abajo de la entrada:

```
{"update",          (PyCFunction)set_update,        METH_VARARGS,
```

### Lists

Para agregar nuevos métodos, solo tenemos que modificar
la estructura `static PyMethodDef list_methods[]`, y
se puede copiar el código de, por ejemplo, el método append
y convertirlo en un append doble.

Declarando función

```
static PyObject *
list_appendappend(PyListObject *self, PyObject *object)
{
    app1(self, object);
    app1(self, object);
    Py_RETURN_NONE;
}
```
que es copiando la implementación oficial de `list_append`.

Para hacer la función `dneppa` (append al revés),
copiamos el método `insert` pero fijando el índice en 0:

```
static PyObject *
list_dneppa(PyListObject *self, PyObject *object)
{
    if (ins1(self, 0, object) == 0)
        Py_RETURN_NONE;
    return NULL;
}
```

Ver [el parche](0001-Parches-para-PyConES.patch) para los otros cambios a la
estructura y la documentación.

### Object

La interacción con el intérprete, cuando se ve texto automáticosiendo impreso
en pantalla, escribí una función simple que se llama `slow_print`:

```
static void slow_print(char *in)
{
    char *i;
    for (i = in; *i != '\0'; i++) {
        printf("%c", *i);
        fflush(stdout);
        if (*i == ',')
            usleep(400000);
        else
            usleep(70000);
    }
    printf("\n");
}
```

luego en el método `.pregunta()` solo hace comparación de
strings para ver que contestar:

```
static PyObject *
pregunta(PyListObject *self, PyObject *object)
{
    PyObject* str = PyUnicode_AsUTF8String(object);
    const char *bytes = PyBytes_AsString(str);

    if (!strcmp(bytes, "¿cómo estás?"))
    {
        slow_print("Mmm...bien, con nervios por el release de 3.10 en 2 días...pero bien");
    }
    else if (!strcmp(bytes, "¿eres ML?"))
    {
        slow_print("Claro!, estoy en constante 'Training'...");
        system("sl");
        slow_print("Ja! entendiste?, un 'Train' por 'Training'...");
        slow_print("No tengo buenos chistes... :(");
    }
    else if (!strcmp(bytes, "¿puedo preguntar lo que sea?"))
    {
        slow_print("Por supuesto, para eso estoy");
    }
    else if (!strcmp(bytes, "¿lleva cebolla la tortilla de patata?"))
    {
        printf("Traceback (most recent call last):\n");
        printf("  File \"<stdin>\", line 1, in <module>\n");
        printf("BlaBlaError: Al objeto le dio miedo responder\n\n");
    }
    else {
        slow_print("uh?");
    }

    Py_XDECREF(str);


    Py_RETURN_NONE;
}
```


### Builtins

Tanto el caso especial de `l.append("red")` (o con `green` y `blue`) como el
módo Twitch, solo llaman `obs-cli` como llamada del sistema.
Para ello hay que configurarlo en cada ordenador y configurar
las distintas escenas.

```
static PyObject*
twitch(PyObject *self)
{
    system("obs-cli scene switch 'Scene Twitch' --password 'vaso con agua'");
    Py_RETURN_NONE;
}
```

(La configuración de `obs-cli` requiere establecer una password, por eso se
pasa como argumento)

Para el caso de la división por cero,
se utilizó solo el caso para `longobject` es decir números
enteros,
principalmente la función `long_true_divide` cambiando: `"division by zero"`
por:

```
"\n __________________ "
"\n< division by zero >"
"\n ------------------ "
"\n        \\   ^__^"
"\n         \\  (oo)\\_______"
"\n            (__)\\       )\\/\\"
"\n                ||----w |"
"\n                ||     ||"
                        );
        if (PyLong_AsLong(v) == 0)
            system("cmatrix");
```

Al final se puede ver que si para `N/0` el valor de `N` también es cero, se
llama al programa `cmatrix` que muestra un texto en modo matrix por la
terminal.

