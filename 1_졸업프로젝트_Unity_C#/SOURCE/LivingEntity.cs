using System;
using UnityEngine;
using Photon.Pun;
using Photon.Realtime;

// 생명체로서 동작할 게임 오브젝트들을 위한 뼈대를 제공
// 체력, 데미지 받아들이기, 사망 기능, 사망 이벤트를 제공
public class LivingEntity : MonoBehaviourPunCallbacks, IDamageable, IPunObservable
{
    public float InitHealth = 100f; // 시작 체력
    public float health;            // 현재 체력
    public bool dead;               // 사망 상태 True : 사망 False : 삶

    // 생명체가 활성화될때 상태를 리셋
    override public void OnEnable()
    {
        dead = false;           // 사망하지 않은 상태로 시작
        health = InitHealth;    // 체력을 시작 체력으로 초기화
    }

    [PunRPC]
    public void ApplyUpdatedHealth(float newHealth)
    {
        health = newHealth;
    }

    // 데미지를 입는 기능
    [PunRPC]
    virtual public void OnDamage(float damage)
    {
        if(PhotonNetwork.IsMasterClient)
        {
            health -= damage;   // 데미지만큼 체력 감소
            photonView.RPC("ApplyUpdatedHealth", RpcTarget.Others, health);
            photonView.RPC("OnDamage", RpcTarget.Others, damage);
        }

        if (health <= 0 && !dead)
        {
            // 체력이 0 이하 && 아직 죽지 않았다면 사망 처리 실행
            Die();
        }
    }

    // 체력을 회복하는 기능
    [PunRPC]
    virtual public void RestoreHealth(float newHealth)
    {
        if (dead)
        {
            // 이미 사망한 경우 체력을 회복할 수 없음
            return;
        }
        
        if (PhotonNetwork.IsMasterClient)
        {
            // 체력 추가
            health += newHealth;
            if (health > InitHealth)
                health = InitHealth;

            photonView.RPC("ApplyUpdatedHealth", RpcTarget.Others, health);
        }
    }

    // 사망 처리
    virtual public void Die()
    {
        dead = true;
    }

    // 포톤을 통해 체력 상태 공유
    virtual public void OnPhotonSerializeView(PhotonStream stream, PhotonMessageInfo info)
    {
        if (stream.IsWriting)
        {
            stream.SendNext(health);
        }
        else
        {
            health = (float)stream.ReceiveNext();

            if (health <= 0 && !dead)
            {
                Die();
            }
        }
    }
}
