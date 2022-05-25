def gen_get_set(tp, name):
    nm = name[2:]    
    set_str = 'inline void Set' + nm + '(const ' + tp + '& value) {' + name + ' = value;}'
    get_str = 'inline ' + tp + ' Get' + nm + '() const {return ' + name + ';}'
    return set_str, get_str

def get_tp_and_name(var_lines:str):
    var_li = [var.strip('\n\t ') for var in var_lines.split(';')] 
    return var_li

def gen(var_lines:str):
    var_li = get_tp_and_name(var_lines)
    for var in var_li:
        if var == '':
            continue
        tmp = var.split(' ')
        tp, name = tmp[0], tmp[1]
        s, g = gen_get_set(tp, name)
        print(s)
        print(g)
        print()

s = """		glm::vec3 m_Position;
		glm::vec3 m_Front;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_WorldUp;
		float m_Yaw;
		float m_Pitch;
		float m_MovementSpeed;
		float m_MoveSensitivity;
		float m_ZNear;
		float m_ZFar;
		float m_DegFOV;
		float m_Aspect;"""

gen(s)