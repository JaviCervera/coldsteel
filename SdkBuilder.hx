import sys.FileSystem;
import sys.io.File;

using Lambda;
using StringTools;

class SdkBuilder {
	public static function main():Void {
		final root = Xml.parse(File.getContent("coldsteel.xml")).firstElement();
		if (!FileSystem.isDirectory("_build/sdk"))
			FileSystem.createDirectory("_build/sdk");
		File.saveContent("_build/sdk/sdk.h", getSdkContent(root));
		File.saveContent("src/internal/get_sdk_impl.h", getCreateSdkContent(root));
	}

	private static function getSdkContent(root:Xml):String {
		final functions = getFunctions(root);
		return getSdkHeader() + getSdkTypes(functions).map(t -> '$t\n').join("") + getSdkHeader2() + getSdkFunctions(functions).map(f -> '    $f').join("\n")
			+ getSdkFooter();
	}

	private static function getCreateSdkContent(root:Xml):String {
		return getCreateSdkHeader() + getCreateSdkFunctions(getFunctions(root)).map(f -> '  $f\n').join("") + getCreateSDKFooter();
	}

	private static function getFunctions(element:Xml):Iterable<Function> {
		final functions = XmlParser.elements(element, 'cdecl').filter(c -> XmlParser.attribute(c, 'kind') == 'function').map(f -> Function.fromXml(f));
		final childFunctions = XmlParser.elements(element).flatMap(c -> getFunctions(c));
		return functions.concat(childFunctions).filter(f -> !f.name.startsWith('_'));
	}

	private static function getSdkTypes(funcs:Iterable<Function>):Array<String> {
		final excluded = ["void", "int", "float", "const char *"];
		final types = new Array<String>();
		for (f in funcs) {
			final cppType = getCppType(f.type);
			if (!excluded.contains(cppType) && !types.contains(cppType))
				types.push(cppType);
			f.params.iter(p -> if (!excluded.contains(cppType) && !types.contains(cppType)) types.push(cppType));
		}
		final result = new Array<String>();
    for (t in types)
      if (t == 'Memblock *')
        result.push("typedef void Memblock;");
      else
        result.push(t.endsWith("*") ? 'struct ${t.substr(0, t.length - 1).trim()};' : 'typedef int $t;');
    for (f in funcs)
      result.push('typedef ${getCppFuncType(f.type)}(*${f.name}Func)(${getSdkParams(f.params)});');
    return result;
	}

	private static function getSdkFunctions(funcs:Iterable<Function>):Array<String> {
		return funcs.map(f -> '${f.name}Func ${f.name};');
	}

	private static function getSdkParams(params:Iterable<Param>):String {
		return params.map(p -> getCppType(p.type)).join(", ");
	}

  private static function getCreateSdkFunctions(funcs:Iterable<Function>):Array<String> {
    return funcs.map(f -> 'sdk.${f.name} = (${f.name}Func)${f.name};');
  }

	private static function getCppFuncType(srcType:String):String {
		final type = getCppType(srcType);
		final space = type.endsWith("*") ? "" : " ";
		return '$type$space';
	}

	private static function getCppType(srcType:String):String {
		return switch (srcType) {
			case "Void": return "void";
			case "Bool": return "Bool";
			case "Int": return "int";
			case "Float": return "float";
			case "String": return "const char *";
			case "Channel": return "Channel";
			case "IBillboardSceneNode": return "Sprite *";
			case "ICameraSceneNode": return "Camera *";
			case "IImage": return "Pixmap *";
			case "ILightSceneNode": return "Light *";
			case "IMesh": return "Mesh *";
			case "IMeshBuffer": return "Surface *";
			case "ISceneNode": return "Entity *";
			case "IMeshSceneNode": return "Model *";
			case "ITerrainSceneNode": return "Terrain *";
			case "ITexture": return "Texture *";
			case "SMaterial": return "Material *";
			default: return '${srcType} *';
		};
	}

	private static function getSdkHeader():String {
		return "
#ifndef COLDSTEEL_SDK_H_INCLUDED
#define COLDSTEEL_SDK_H_INCLUDED

#ifdef __cplusplus
extern \"C\"
{
#endif

";
	}

	private static function getSdkHeader2():String {
		return "
  typedef struct
  {
    void (*RegisterFunction)(const char *name, int (*f)(void *));
    int (*GetBoolArg)(void *context, int index);
    int (*GetIntArg)(void *context, int index);
    float (*GetFloatArg)(void *context, int index);
    const char *(*GetStringArg)(void *context, int index);
    const void *(*GetPointerArg)(void *context, int index);
    void (*PushBool)(void *context, int val);
    void (*PushInt)(void *context, int val);
    void (*PushFloat)(void *context, float val);
    void (*PushString)(void *context, const char *val);
    void (*PushPointer)(void *context, void *val);
";
	}

	private static function getSdkFooter():String {
		return "
} ColdSteelSDK;

#ifdef __cplusplus
} /* extern \"C\" */
#endif

#endif /* COLDSTEEL_SDK_H_INCLUDED */
";
	}

	private static function getCreateSdkHeader():String {
		return "
inline ColdSteelSDK Scripting_Lua::GetSDK()
{
  ColdSteelSDK sdk;
  sdk.RegisterFunction = &Scripting_Lua::RegisterFunction;
  sdk.GetBoolArg = &Scripting_Lua::GetBoolArg;
  sdk.GetIntArg = &Scripting_Lua::GetIntArg;
  sdk.GetFloatArg = &Scripting_Lua::GetFloatArg;
  sdk.GetStringArg = &Scripting_Lua::GetStringArg;
  sdk.GetPointerArg = &Scripting_Lua::GetPointerArg;
  sdk.PushBool = &Scripting_Lua::PushBool;
  sdk.PushInt = &Scripting_Lua::PushInt;
  sdk.PushFloat = &Scripting_Lua::PushFloat;
  sdk.PushString = &Scripting_Lua::PushString;
  sdk.PushPointer = &Scripting_Lua::PushPointer;
";
	}

	private static function getCreateSDKFooter():String {
		return "  return sdk;
}
";
	}
}

private class Function {
	public final name:String;
	public final type:String;
	public final params:Iterable<Param>;

	public function new(name:String, type:String, params:Iterable<Param>) {
		this.name = name;
		this.type = type;
		this.params = params.array();
	}

	public static function fromXml(element:Xml):Function {
		final attributelistElem = XmlParser.elements(element, 'attributelist')[0];
		final parmlistElem = XmlParser.elements(attributelistElem, 'parmlist')[0];
		final params = (parmlistElem != null) ? XmlParser.elements(parmlistElem, 'parm') : [];
		return new Function(XmlParser.attribute(element, 'name'), XmlParser.type(XmlParser.attribute(element, 'type')), params.map(p -> Param.fromXml(p)));
	}

	public function toString():String {
		return '$name(${params.map(p -> p.toString()).join(', ')}):$type';
	}
}

private class Param {
	public final name:String;
	public final type:String;

	public function new(name:String, type:String) {
		this.name = name;
		this.type = type;
	}

	public static function fromXml(element:Xml):Param {
		return new Param(XmlParser.attribute(element, 'name'), XmlParser.type(XmlParser.attribute(element, 'type')));
	}

	public function toString():String {
		return '$name:$type';
	}
}

private class XmlParser {
	public static function attribute(element:Xml, name:String):String {
		final attributes = element.elementsNamed('attributelist').next().elementsNamed('attribute');
		for (attr in attributes) {
			if (attr.get('name') == name) {
				return attr.get('value');
			}
		}
		return '';
	}

	public static function elements(root:Xml, ?name:String):Array<Xml> {
		final it = (name != null) ? root.elementsNamed(name) : root.elements();
		return [for (el in it) el];
	}

	public static function type(type:String):String {
		if (type == 'void') {
			return 'Void';
		} else if (type == 'bool' || type == 'bool_t') {
			return 'Bool';
		} else if (type == 'int') {
			return 'Int';
		} else if (type == 'float') {
			return 'Float';
		} else if (type == 'p.q(const).char' || type == 'q(const).char') {
			return 'String';
		} else if (type.startsWith('p.q(const).')) {
			return type.substr(11);
			return type;
		} else if (type.startsWith('p.')) {
			return type.substr(2);
		} else {
			return type;
		}
	}
}
